#include <stdio.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnx_tcp_socket.h"
#include "jnxthread.h"
#include <assert.h>
#include "jnxthread.h"
#include <time.h>
#define TCPTESTPORT "9096"

static int test_tcp_listener_complete = 0;

static void *worker(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_tcp_create(AF_INET);
  jnx_socket_tcp_send(t,"127.0.0.1",port,"ping",5);

  return NULL;
}
static void *worker_ipv6(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_tcp_create(AF_INET6);
  jnx_socket_tcp_send(t,"::1",port,"ping",5);

  return NULL;
}
static void fire_threaded_tcp_packet(char *port) {
  jnx_thread_create_disposable(worker,port);
}
static void fire_threaded_tcp_packet_ipv6(char *port) {
  jnx_thread_create_disposable(worker_ipv6,port);
}
static void test_tcp_listener_callback(const jnx_uint8 *payload, \
      jnx_size bytes_read, int connected_socket, void *args){
  JNXLOG(LDEBUG,"test_tcp_listener_callback achieved");
  test_tcp_listener_complete = 1;
}
static void test_tcp_listener() {
  jnx_tcp_listener *listener = 
    jnx_socket_tcp_listener_create(TCPTESTPORT,AF_INET,100);

  fire_threaded_tcp_packet(TCPTESTPORT);
  int x = 0;
  while(x < 20) {
    jnx_socket_tcp_listener_tick(listener,test_tcp_listener_callback,NULL);
    if(test_tcp_listener_complete)break;
    ++x;
  }
  jnx_socket_tcp_listener_destroy(&listener);
  JNXCHECK(test_tcp_listener_complete);
  JNXCHECK(listener == NULL);
}
static void test_tcp_listener_ipv6() {
  jnx_tcp_listener *listener = 
    jnx_socket_tcp_listener_create(TCPTESTPORT,AF_INET6,100);
  fire_threaded_tcp_packet_ipv6(TCPTESTPORT);
  int x = 0;
  while(x < 20) {
    jnx_socket_tcp_listener_tick(listener,test_tcp_listener_callback,NULL);
    if(test_tcp_listener_complete)break;
    ++x;
  }
  jnx_socket_tcp_listener_destroy(&listener);
  JNXCHECK(test_tcp_listener_complete);
  JNXCHECK(listener == NULL);
}
static void test_blocking_listener_callback(const jnx_uint8 *payload, \
      jnx_size bytes_read, int connected_socket, void *args){
  test_tcp_listener_complete = 1;
}
static void *worker_blocking_listener(void *args) {
  jnx_tcp_listener **listener = args;
  *listener = jnx_socket_tcp_listener_create(TCPTESTPORT,
      AF_INET,100);
  jnx_socket_tcp_listener_auto_tick(*listener,test_blocking_listener_callback,
      NULL);

  return NULL;
}
static void test_tcp_blocking_listener() {
  jnx_tcp_listener *listener;

  jnx_thread_create_disposable(worker_blocking_listener,&listener);
  sleep(1);
  fire_threaded_tcp_packet(TCPTESTPORT);
  clock_t start = clock(),diff;
  jnx_float msec = 0;
  while(msec < 500 && !test_tcp_listener_complete){
    diff = clock();
    msec = (diff - start) * 1000 / CLOCKS_PER_SEC;
  }
  listener->hint_exit = 1;
  JNXCHECK(test_tcp_listener_complete);
}
int test_jnxtcpsocket() {
  JNXLOG(LDEBUG,"Starting tcp socket tests");
  JNXLOG(LDEBUG,"Testing TCP Listener");
  test_tcp_listener();
  test_tcp_listener_complete = 0;
  JNXLOG(LDEBUG,"Testing TCP Listener IPV6");
  test_tcp_listener_ipv6();
  test_tcp_listener_complete = 0;
  JNXLOG(LDEBUG,"Test TCP blocking listener");
  test_tcp_blocking_listener();
  return 0;
}
