#include "TCPClientMD.h"

// Define the structs and the enums here
enum tcp_client_state{
  TCS_NONE = 0, 
  TCS_CONNECTED =1,
  TCS_RECIEVING = 2,
  TCS_CLOSING = 3
};

typedef struct {
  u8_t state;
  u8_t retries;
  struct tcp_pcb *pcb;
  struct pbuf *p_buffer;
  char *send_message;
  uint32_t send_message_len;
} tcp_client_struct;

// Write the functions deceleration here
static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t tcp_client_recieve(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);
static void tcp_client_send(struct tcp_pcb *tpcb, tcp_client_struct *TC);
static void tcp_client_connection_close(struct tcp_pcb *tpcb, tcp_client_struct *TC);
static void tcp_client_handle(struct tcp_pcb *tpcb, tcp_client_struct *TC);

// Write the function definitions here

void tcp_client_init(void)
{
  struct tcp_pcb *tpcb;
  tpcb = tcp_new();
  
  ip_addr_t servert_ip_addr;
  IP_ADDR4(&servert_ip_addr, 192,168,0,20);
  tcp_connect(tpcb, &servert_ip_addr, 7, tcp_client_connected); 
}

static err_t tcp_client_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  err_t ret_error;
  tcp_client_struct *TC;
  
  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  TC = (tcp_client_struct *)mem_malloc(sizeof(tcp_client_struct));
  if (TC != NULL)
  {
	  TC -> state = TCS_CONNECTED;
	  TC -> pcb = tpcb;
	  TC -> retries = 0;
	  TC ->p_buffer = NULL;
	  TC -> send_message = NULL;

	  tcp_arg(tpcb, TC);
	  tcp_recv(tpcb, tcp_client_recieve);
	  tcp_poll(tpcb, tcp_client_poll, 0);
	  tcp_sent(tpcb, tcp_client_sent);
	  tcp_client_handle(tpcb, TC);

	  ret_error = ERR_OK;
  }
  else
  {
	  tcp_client_connection_close(tpcb, TC);
	  ret_error = ERR_MEM;
  }

  return ret_error;
}


static err_t tcp_client_recieve(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	err_t ret_error;
	tcp_client_struct *TC;

	TC = (tcp_client_struct *)arg;

	if (p == NULL)
	{
		TC -> state = TCS_CLOSING;
		if (TC -> send_message == NULL) {tcp_client_connection_close(tpcb, TC);}
		else{tcp_client_send(tpcb, TC);}
	}

	else if(err != ERR_OK)
	{
		if(p != NULL) {TC -> p_buffer = NULL; pbuf_free(p);}
		ret_error = err;
	}

	else if (TC -> state == TCS_CONNECTED)
	{
		TC -> p_buffer = p;
		tcp_recved(tpcb, p ->tot_len);
                TC -> state = TCS_RECIEVING;
		tcp_client_handle(tpcb, TC);
		pbuf_free(p);
		ret_error = ERR_OK;
	}
        else if (TC -> state == TCS_RECIEVING)
        {
                TC -> p_buffer = p;
                tcp_recved(tpcb, p ->tot_len);
                tcp_client_handle(tpcb, TC);
                pbuf_free(p);
                ret_error = ERR_OK;
        }

	else if (TC -> state == TCS_CLOSING)
	{
		tcp_recved(tpcb, p->tot_len);
		TC -> p_buffer = NULL;
		TC -> send_message = NULL;
		pbuf_free(p);
		ret_error = ERR_OK;
	}

	else
	{
		tcp_recved(tpcb, p->tot_len);
		TC -> p_buffer = NULL;
		TC -> send_message = NULL;
		pbuf_free(p);
		ret_error = ERR_OK;
	}

	return ret_error;
}


static err_t tcp_client_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_error;
	tcp_client_struct *TC;

	TC = (tcp_client_struct *)arg;
	if (TC != NULL)
	{
		if (TC -> send_message != NULL) {tcp_client_send(tpcb, TC);}
		else {if (TC -> state == TCS_CLOSING) {tcp_client_connection_close(tpcb, TC);}}
		ret_error = ERR_OK;
	}
	else
	{
		tcp_abort(tpcb);
		ret_error = ERR_ABRT;
	}

	return ret_error;
}


static err_t tcp_client_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	tcp_client_struct *TC;
	LWIP_UNUSED_ARG(len);

	TC = (tcp_client_struct *)arg;
	TC -> retries = 0;

	if(TC -> send_message != NULL){tcp_client_send(tpcb, TC);}
	else
	{
		if (TC -> state == TCS_CLOSING) {tcp_client_connection_close(tpcb, TC);}
	}

	return ERR_OK;
}


static void tcp_client_send(struct tcp_pcb *tpcb, tcp_client_struct *TC)
{
	err_t wr_error;

	if (TC ->send_message != NULL)
	{
		wr_error = tcp_write(tpcb, TC ->send_message, TC ->send_message_len, 1);
		if (wr_error == ERR_OK) {TC ->send_message = NULL; TC -> send_message_len = 0;}
		else {/*Message sending error*/}
	}
	else {/* No message to send*/}
}


static void tcp_client_connection_close(struct tcp_pcb *tpcb, tcp_client_struct *TC)
{
	  tcp_arg(tpcb, NULL);
	  tcp_sent(tpcb, NULL);
	  tcp_recv(tpcb, NULL);
	  tcp_err(tpcb, NULL);
	  tcp_poll(tpcb, NULL, 0);

	  if (TC != NULL) {mem_free(TC);}
	  tcp_close(tpcb);
}


static void tcp_client_handle(struct tcp_pcb *tpcb, tcp_client_struct *TC)
{
	ip4_addr_t inIP = tpcb ->remote_ip;
	uint16_t inPort = tpcb -> remote_port;
	char *remIP = ipaddr_ntoa(&inIP);
        
        uint8_t message_t_send[1024];
        uint16_t message_to_send_len;
        
        if (TC -> state == TCS_CONNECTED)
        {
          message_to_send_len = sprintf(message_t_send, "Microf407VGT6 connected ...\n");
        }
        else
        {
          message_to_send_len = sprintf(message_t_send, "Hi there, thius is Ali. I recieved your message\n");
        }
        
        
        TC -> send_message = message_t_send;
        TC -> send_message_len = message_to_send_len;
        tcp_client_send(tpcb, TC);
}










