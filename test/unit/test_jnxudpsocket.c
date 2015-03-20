#include <stdio.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnx_udp_socket.h"
#include "jnxthread.h"
#include <assert.h>
#include "jnxthread.h"
#include <time.h>
#define TESTPORT "9092"
#define TESTPORT2 "9093"
#define BGROUP "225.1.1.1"
static int test_udp_listener_complete = 0;

void *worker(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_udp_create(AF_INET);
  jnx_socket_udp_send(t,"127.0.0.1",port,"ping",5);
}
void *worker_ipv6(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_udp_create(AF_INET6);
  jnx_socket_udp_send(t,"::1",port,"ping",5);
}
void *worker_broadcast(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_udp_create(AF_INET);
  jnx_socket_udp_broadcast_send(t,"127.0.0.1",port,"ping",5);
}
void *worker_multicast(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_udp_create(AF_INET);
  jnx_socket_udp_send(t,BGROUP,port,"ping",5);
}
void fire_threaded_udp_packet(char *port) {
  jnx_thread_create_disposable(worker,port);
}
void fire_threaded_udp_packet_ipv6(char *port) {
  jnx_thread_create_disposable(worker_ipv6,port);
}
void fire_threaded_udp_packet_broadcast(char *port) {
  jnx_thread_create_disposable(worker_broadcast,port);
}
void fire_threaded_udp_packet_multicast(char *port) {
  jnx_thread_create_disposable(worker_multicast,port);
}
void test_udp_listener_callback(jnx_uint8 *payload,
    jnx_size bytes_read, jnx_socket *s, jnx_int connected_socket,void *args){
  JNX_LOG(NULL,"test_udp_listener_callback achieved");
  test_udp_listener_complete = 1;
}
void test_udp_listener() {
  jnx_udp_listener *listener = 
    jnx_socket_udp_listener_create(TESTPORT,AF_INET);

  fire_threaded_udp_packet(TESTPORT);
  int x = 0;
  while(x < 5) {
    jnx_socket_udp_listener_tick(listener,test_udp_listener_callback,NULL);
    if(test_udp_listener_complete)break;
    ++x;
  }
  jnx_socket_udp_listener_destroy(&listener);
  JNXCHECK(test_udp_listener_complete);
  JNXCHECK(listener == NULL);
}
void test_udp_listener_ipv6() {
  jnx_udp_listener *listener = 
    jnx_socket_udp_listener_create(TESTPORT,AF_INET6);
  fire_threaded_udp_packet_ipv6(TESTPORT);
  int x = 0;
  while(x < 5) {
    jnx_socket_udp_listener_tick(listener,test_udp_listener_callback,NULL);
    if(test_udp_listener_complete)break;
    ++x;
  }
  jnx_socket_udp_listener_destroy(&listener);
  JNXCHECK(test_udp_listener_complete);
  JNXCHECK(listener == NULL);
}

void test_udp_broadcast(){
  jnx_udp_listener *listener = 
    jnx_socket_udp_listener_broadcast_create(TESTPORT,AF_INET);

  fire_threaded_udp_packet_broadcast(TESTPORT);
  int x = 0;
  while(x < 5) {
    jnx_socket_udp_listener_tick(listener,test_udp_listener_callback,NULL);
    if(test_udp_listener_complete)break;
    ++x;
  }
  jnx_socket_udp_listener_destroy(&listener);
  JNXCHECK(test_udp_listener_complete);
  JNXCHECK(listener == NULL);
}
void test_blocking_listener_callback(jnx_uint8 *payload,
    jnx_size bytes_read, jnx_socket *s, jnx_int connected_socket,void *args){
  test_udp_listener_complete = 1;
}
void worker_blocking_listener(void *args) {
  jnx_udp_listener **listener = args;
  *listener = jnx_socket_udp_listener_create(TESTPORT,
      AF_INET);
  jnx_socket_udp_listener_auto_tick(*listener,test_blocking_listener_callback,
      NULL);
}
void test_udp_blocking_listener() {
  jnx_udp_listener *listener;

  jnx_thread_create_disposable(worker_blocking_listener,&listener);
  sleep(1);
  fire_threaded_udp_packet(TESTPORT);
  clock_t start = clock(),diff;
  jnx_float msec = 0;
  while(msec < 500 && !test_udp_listener_complete){
    diff = clock();
    msec = (diff - start) * 1000 / CLOCKS_PER_SEC;
  }
  listener->hint_exit = 1;
  jnx_socket_udp_listener_destroy(&listener);
  JNXCHECK(test_udp_listener_complete);
  JNXCHECK(listener == NULL);
}
void test_udp_multicast(){
  jnx_udp_listener *listener = 
    jnx_socket_udp_listener_multicast_create(TESTPORT2,AF_INET,BGROUP);
  
  fire_threaded_udp_packet_multicast(TESTPORT2);
  int x = 0;
  while(x < 5) {
    jnx_socket_udp_listener_tick(listener,test_udp_listener_callback,NULL);
    if(test_udp_listener_complete)break;
    ++x;
  }
  jnx_socket_udp_listener_destroy(&listener);
  JNXCHECK(test_udp_listener_complete);
  JNXCHECK(listener == NULL);

}
int main(int argc, char **argv) {
  JNX_LOG(NULL,"Starting udp socket tests");
  JNX_LOG(NULL,"Testing UDP listener");
  test_udp_listener();
  test_udp_listener_complete = 0;
  JNX_LOG(NULL,"Testing UDP listener IPV6");
  test_udp_listener_ipv6();
  test_udp_listener_complete = 0;
  JNX_LOG(NULL,"Testing UDP broadcast");
  test_udp_broadcast(); 
  test_udp_listener_complete = 0;
  JNX_LOG(NULL,"Test UDP blocking listener");
  test_udp_blocking_listener();
  test_udp_listener_complete = 0;
  JNX_LOG(NULL,"Test UDP multicast listener");
  test_udp_multicast();
  return 0;
}
