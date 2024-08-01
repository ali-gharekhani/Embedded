#include "TCPServerMD.h"
#include "SDFileHandling.h"

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
static err_t tcp_server_receive(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_server_handle (struct tcp_pcb *tpcb,  tcp_server_struct *TS);
static void tcp_server_send(struct tcp_pcb *tpcb,  tcp_server_struct *TS);
static void tcp_server_connection_close(struct tcp_pcb *tpcb, tcp_server_struct *TS);
static void tcp_server_error(void *arg, err_t err);
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);

u8_t freed;
struct pbuf *ptr;
int test = 0;
char recv_buffer[100];

/* This function will perform three main functions namely: 
Binding the host IP and port - Listenning to the binded port - Accept the connection request*/
void tcp_server_start(void){
  struct tcp_pcb *tcp_block;
  tcp_block = tcp_new();

  err_t server_status;
  
  ip_addr_t myIP;
  IP_ADDR4(&myIP, 192,168,0,156);
  uint16_t port =7;
  
  server_status = tcp_bind(tcp_block, &myIP, port);
  if (server_status == ERR_OK){
    tcp_block = tcp_listen(tcp_block);
    tcp_accept(tcp_block, tcp_server_accept);
  }else{
    //ToDO: 
  }
}

/* This function will accept the connection request if everything was OK.
A tcp_server_struct will be assigned dynamically to the new connection.*/
static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err){

  tcp_server_struct *TS;
  err_t ret_error;
  
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);
  
  TS = (tcp_server_struct*)mem_malloc(sizeof(tcp_server_struct));
  if (TS != NULL){
    TS -> state = TSS_ACCEPTED;
    TS -> pcb = newpcb;
    TS -> retries = 0;
    TS -> message_buffer = NULL;
    
    tcp_arg(newpcb, TS);
    tcp_recv(newpcb, tcp_server_receive);
    tcp_err(newpcb, tcp_server_error);
    tcp_poll(newpcb, tcp_server_poll, 0);
    
    ret_error = ERR_OK;
  }else{
    ret_error = ERR_MEM;
  }
  
  return ret_error;
}

/* This function runs when a the server receives a message over 
the connection. Here you should decide what you want to do after
the received message or whit that.*/
static err_t tcp_server_receive(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err){
  
  tcp_server_struct *TS;
  TS = (tcp_server_struct*)arg;
  
  err_t ret_error;
  
  // Remote has closed the connection
  if (p ==NULL){
    TS -> state = TSS_CLOSING;
    if (TS -> send_message == NULL){tcp_server_connection_close(tpcb, TS); unmount_sd();}
    else{tcp_sent(tpcb, tcp_server_sent);       tcp_server_send(tpcb, TS);}
    return ret_error;
  }
  
  else if(err != ERR_OK){
    if (p != NULL) {TS -> recv_message = NULL; pbuf_free(p);}
    ret_error = err; 
  }
  
  else if (TS -> state == TSS_ACCEPTED){
    mount_sd();
    TS -> state = TSS_RECEIVED;
    TS -> message_buffer = p;
    
    tcp_sent(tpcb, tcp_server_sent);
    tcp_server_handle(tpcb, TS);
    
    pbuf_free(p);
    ret_error = ERR_OK;
  }
  
  else if (TS -> state == TSS_RECEIVED){
    if (TS -> send_message == NULL){
      strncpy(TS -> recv_message, (char*)p ->payload, p->tot_len);
      TS -> message_buffer = p;
      
      tcp_server_handle(tpcb, TS);
      pbuf_free(p);
    }
    else {tcp_server_send(tpcb, TS); tcp_server_handle(tpcb, TS);}
    return ERR_OK;
  }
  
  else if (TS -> state == TSS_CLOSING){
    tcp_recved(tpcb, p->tot_len);
    TS -> message_buffer = NULL;
    TS -> send_message = NULL;
    TS -> recv_message = NULL;
    pbuf_free(p);
    ret_error = ERR_OK;
    unmount_sd();
  }
  
  else{
    tcp_recved(tpcb, p->tot_len);
    TS->message_buffer = NULL;
    TS -> send_message = NULL;
    TS -> recv_message = NULL;
    pbuf_free(p);
    ret_error = ERR_OK;
    unmount_sd();
  }
  
  return ret_error;
}

/* This function runs when a message (successfully) has been sent .
Here you should decide what to do after sending the message over the
connection (successfully).*/
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
  tcp_server_struct *TS;
  LWIP_UNUSED_ARG(len);
  
  TS = (tcp_server_struct*)arg;
  TS -> retries = 0;
  
  if (TS -> send_message != NULL){
    tcp_sent(tpcb, tcp_server_sent); 
    tcp_server_send(tpcb, TS);}
  else{
  if (TS -> state == TSS_CLOSING){tcp_server_connection_close(tpcb, TS);}
  
  }
  return ERR_OK;
}

/* This is a callback function which you can use it anywhwre you want.
In this case it has been used as the received message callback function.*/
static void tcp_server_handle (struct tcp_pcb *tpcb,  tcp_server_struct *TS){
  
  ip4_addr_t inIP = tpcb -> remote_ip;
  uint16_t inPort = tpcb -> remote_port;
  char *remote_ip = ipaddr_ntoa(&inIP);
  
  uint8_t send_message[1024];
  uint16_t message_len;
  message_len = sprintf(send_message, "Hello threre. This message has been sent by ali gharekhani via ethernet");
  TS -> send_message = send_message;
  TS -> send_message_len = message_len;
  
 
//  strncpy(TS -> recv_message, (char*)TS->message_buffer ->payload, TS -> message_buffer->tot_len);
  char write_buffer[100];
  

  sd_create_file("test.txt");
  strncpy(write_buffer, (char*)TS->message_buffer ->payload, TS -> message_buffer->len);
  strcat(write_buffer, "\n");
  sd_write_file("test.txt", write_buffer);
  
  memset(write_buffer, '\0', strlen(write_buffer));
 
  tcp_server_send(tpcb, TS);
  
  TS -> recv_message = NULL; 
  TS -> message_buffer = NULL;
  
  
}

/* This function handles the basic operation of data sending on communication
line. Here, any error related to the sending data which may occure will be 
checked. This function could be used any where you wnat to send data.*/
static void tcp_server_send(struct tcp_pcb *tpcb,  tcp_server_struct *TS){
  
  err_t ret_error = ERR_OK;
  
  ret_error = tcp_write(tpcb, TS -> send_message, TS -> send_message_len, 0);
  if (ret_error == ERR_OK){TS -> send_message = NULL;}
//  while ((ret_error == ERR_OK) && (TS -> message_buffer != NULL) && (TS -> message_buffer -> len <= tcp_sndbuf(tpcb)))
//  {
//    ptr = TS -> message_buffer;
//    ret_error = tcp_write(tpcb, TS -> send_message, TS -> send_message_len, 0);
//    
//    if (ret_error == ERR_OK)
//    {
//      u16_t plen; 
//      plen = ptr -> len;
//      if (ptr -> next != NULL){
//        TS -> message_buffer = ptr -> next;}
//      
//      if (TS -> message_buffer != NULL){pbuf_ref(TS -> message_buffer);}
//      do {freed = pbuf_free(ptr);} while (freed == 0);
//      tcp_recved(tpcb, plen);
//
//      
//    }
//    else if (ret_error == ERR_MEM){TS -> message_buffer = ptr;}
//    else{/* Other problems */}
//  }

}

/* This function closes the connection whenever you want. Additionally this 
frees the defined structures and functions.*/
static void tcp_server_connection_close(struct tcp_pcb *tpcb, tcp_server_struct *TS){
  // Remove all callbacks
  tcp_arg(tpcb, NULL);
  tcp_sent(tpcb, NULL);
  tcp_recv(tpcb, NULL);
  
  // Free the memory alloc ated to the server struct
  if (TS != NULL){
    mem_free(TS);
  }
  
  // Close the connection
  tcp_close(tpcb);
}

static void tcp_server_error(void *arg, err_t err){
  tcp_server_struct *TS;
  LWIP_UNUSED_ARG(err);
  
  TS = (tcp_server_struct*)arg;
  if (TS != NULL){
    mem_free(TS);
  }

}


static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb){
  err_t ret_error;
  tcp_server_struct *TS;
  
  TS = (tcp_server_struct *)arg;
  if (TS != NULL){
    if (TS -> send_message != NULL){tcp_sent(tpcb, tcp_server_sent); tcp_server_send(tpcb, TS);}
    else{
      if (TS -> state == TSS_CLOSING) {tcp_server_connection_close(tpcb, TS);}
    }
    ret_error = ERR_OK; 
  }
  else{tcp_abort(tpcb); ret_error = ERR_ABRT; }
  
  return ret_error; 
}




