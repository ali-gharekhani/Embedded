#ifndef INC_TCPSERVERMD_H_
#define INC_TCPSERVERMD_H_

#include "main.h"
#include "lwip/tcp.h"
#include "pbuf.h"
#include "string.h"

enum tcp_server_status {
  TSS_NONE = 0,
  TSS_ACCEPTED = 1,
  TSS_RECEIVED = 2,
  TSS_CLOSING = 3
};

typedef struct {
  uint8_t state;
  uint8_t retries;
  struct tcp_pcb *pcb;
  struct pbuf *message_buffer;
  char *recv_message;
  char *send_message; 
  uint16_t send_message_len;

}tcp_server_struct;

void tcp_server_start(void);

#endif