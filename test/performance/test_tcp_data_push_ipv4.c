/*
 * =====================================================================================
 *
 *       Filename:  ping.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/17/14 13:09:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxsocket.h"
#include "jnxfile.h"
#include "jnxnetwork.h"
#include "jnxlog.h"
#include "jnxencoder.h"
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define TESTASSET "assets/image.png"
#define ASSETPATH "assets"
#define SAVEDEFINITION "output/saved-image-%d.png"
int count = 0;
ssize_t total_bytes = 0;
jnx_encoder *en;

void send_msg(char *buffer) {
  jnx_socket *s = jnx_socket_tcp_create(AF_INET);
  jnx_socket_tcp_send(s,"127.0.0.1","9090",buffer,strlen(buffer));
  jnx_socket_destroy(&s);
}

void *run_sender() {
  time_t st;
  time(&st);
  time_t ed;
  int max_time = 5.0;
  int current_time = 0.0;

  jnx_encoder *en = jnx_encoder_create();
  char *obuffer;
  size_t bytes_read = jnx_file_read(TESTASSET,&obuffer,"r");
  size_t out;
  uint8_t *msg = jnx_encoder_b64_encode(en,obuffer,bytes_read + 1,&out);
  while(current_time < max_time) {
    send_msg(msg);
    time(&ed);
    current_time = (ed - st);
  }
  return 0;
}
int callback(uint8_t *msg, size_t bytesread, jnx_socket *s) {
  size_t out;
  uint8_t *m = jnx_encoder_b64_decode(en,msg,bytesread,&out);
  char buffer[1024 * 65];	
  bzero(buffer,1024 * 65);
  sprintf(buffer,SAVEDEFINITION,count);
  total_bytes += jnx_file_write(buffer,m,bytesread,"w");
  ++count;
  free(m);
  free(msg);
  return 0;
}
void *run_listener(void *args) {
  en = jnx_encoder_create();
  jnx_socket *listener = jnx_socket_tcp_create(AF_INET);
  jnx_socket_tcp_listen(listener,"9090",100,callback);
  return 0;
}
int main(int argc, char **argv) {
  time_t st;
  time(&st);
  time_t ed;
  int max_time = 5;

  int current_time =0;
  jnx_thread_create_disposable(run_listener,NULL);	
  jnx_thread_create_disposable(run_sender,NULL);	
  while(current_time < max_time) {
    time(&ed);

    current_time = (ed - st);
  }

  do { total_bytes /= 1024;} 
  while(total_bytes > 1024);

  printf("TCP/IP - Received and saved an average of %zuMB per second total %d messages\n",(total_bytes / max_time),count);
  return 0;
}
