#include <stdio.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnx_tcp_socket.h"
#include "jnxthread.h"
#include <assert.h>
#include "jnxthread.h"
#include <time.h>
#define TESTPORT "9092"

static int test_tcp_listener_complete = 0;

void *worker(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_tcp_create(AF_INET);
  jnx_socket_tcp_send(t,"127.0.0.1",port,"ping",5);
}
void *worker_ipv6(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_tcp_create(AF_INET6);
  jnx_socket_tcp_send(t,"::1",port,"ping",5);
}
void fire_threaded_tcp_packet(char *port) {
  jnx_thread_create_disposable(worker,port);
}
void fire_threaded_tcp_packet_ipv6(char *port) {
  jnx_thread_create_disposable(worker_ipv6,port);
}
void test_tcp_listener_callback(jnx_uint8 *payload,
    jnx_size bytes_read, jnx_socket *s, jnx_int connected_socket,void *args){
  JNX_LOG(NULL,"test_tcp_listener_callback achieved");
  test_tcp_listener_complete = 1;
  return 0;
}
void test_tcp_listener() {
 jnx_tcp_listener *listener = 
    jnx_socket_tcp_listener_create(TESTPORT,AF_INET,100);

  fire_threaded_tcp_packet(TESTPORT);
  int x = 0;
  while(x < 5) {
  jnx_socket_tcp_listener_tick(listener,test_tcp_listener_callback,NULL);
  if(test_tcp_listener_complete)break;
  ++x;
}
  jnx_socket_tcp_listener_destroy(&listener);
  JNXCHECK(test_tcp_listener_complete);
  JNXCHECK(listener == NULL);
}
void test_tcp_listener_ipv6() {
 jnx_tcp_listener *listener = 
    jnx_socket_tcp_listener_create(TESTPORT,AF_INET6,100);
  fire_threaded_tcp_packet_ipv6(TESTPORT);
  int x = 0;
  while(x < 5) {
  jnx_socket_tcp_listener_tick(listener,test_tcp_listener_callback,NULL);
  if(test_tcp_listener_complete)break;
  ++x;
}
  jnx_socket_tcp_listener_destroy(&listener);
  JNXCHECK(test_tcp_listener_complete);
  JNXCHECK(listener == NULL);
}
int main(int argc, char **argv) {
  JNX_LOG(NULL,"Starting network tests\n");
 
  test_tcp_listener();
  test_tcp_listener_complete = 0;
  test_tcp_listener_ipv6();
  return 0;
}
