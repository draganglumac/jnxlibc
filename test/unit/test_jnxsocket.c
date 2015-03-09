/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/21/14 18:00:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnxsocket.h"
#include "jnxthread.h"
#include <assert.h>
#include <pthread.h>

#define TESTPORT "8831"
#define TESTPORT1 "8832"
#define TESTPORT2 "8833"
#define TESTPORT3 "8834"
#define TESTPORT4 "8835"

void test_create() {
  JNX_LOG(NULL,"Test creation of socket");
  jnx_socket *a = jnx_socket_tcp_create(AF_INET);
  assert(a);
  jnx_socket *b = jnx_socket_tcp_create(AF_INET6);
  assert(b);
  jnx_socket *c = jnx_socket_udp_create(AF_INET);
  assert(c);
  jnx_socket *d = jnx_socket_udp_create(AF_INET6);
  assert(d);

  jnx_socket_destroy(&a);
  jnx_socket_destroy(&b);
  jnx_socket_destroy(&c);
  jnx_socket_destroy(&d);

  assert(a == NULL);
  assert(b == NULL);
  assert(c == NULL);
  assert(d == NULL);
}
static int tcp_ip4_complete = 0;
static jnx_socket *tcp_ipv4_listener;
static int tcp_ip6_complete = 0;
static jnx_socket *tcp_ipv6_listener;

///////////////////TCP IPV4 /////////////////////////////
void assert_tcp(char *msg) {
  JNX_LOG(0, "[DEBUG] msg=%s\n", msg);
  assert(strcmp(msg,"hi") == 0
      || strcmp(msg,"bye") == 0
      || strcmp(msg,"once more for good measure") == 0
      || strcmp(msg,"STOP") == 0);
}
int tcplistenipv4_callback(char *msg, size_t size,jnx_socket *s,int connected_sockfd) {
  assert_tcp(msg);
  if (strcmp(msg, "STOP") == 0) {
    tcp_ip4_complete = 1;
    return 1;
  }
  return 0; 
}
void *test_tcp_listen_ipv4(void *args) {
  tcp_socket_listener_callback c = tcplistenipv4_callback;
  tcp_ipv4_listener = jnx_socket_tcp_create(AF_INET);
  jnx_socket_tcp_listen(tcp_ipv4_listener,TESTPORT,100,c);
  return 0;
}
void setup_tcp_listen_ipv4_test() {
  JNX_LOG(NULL,"Starting TCP IPV4 test");
  pthread_t worker;
  pthread_create(&worker,NULL,test_tcp_listen_ipv4,NULL);

  sleep(2);
  //listener setup lets send a message
  jnx_socket *s = jnx_socket_tcp_create(AF_INET);
  size_t bytes = jnx_socket_tcp_send(s,"127.0.0.1",TESTPORT,"hi",strlen("hi"));
  printf("."); fflush(stdout); sleep(1);
  bytes = jnx_socket_tcp_send(s,"127.0.0.1",TESTPORT,"bye",strlen("bye"));
  printf("."); fflush(stdout); sleep(1);
  bytes = jnx_socket_tcp_send(s,"127.0.0.1",TESTPORT,"once more for good measure",strlen("once more for good measure"));
  printf("."); fflush(stdout); sleep(1);
  bytes = jnx_socket_tcp_send(s,"127.0.0.1",TESTPORT,"STOP",strlen("STOP"));
  printf(".\n");
  jnx_socket_destroy(&s);
  time_t st;
  time(&st);
  int max = 5;
  float elapsed = 0;
  while(tcp_ip4_complete == 0 && elapsed < max) {

    time_t e;
    time(&e);
    elapsed += (e-st);
  }
  jnx_socket_destroy(&tcp_ipv4_listener);
}

///////////////////TCP IPV6 /////////////////////////////
int tcplistenipv6_callback(char *msg, size_t size,jnx_socket *s,int connected_sockfd) {
  assert_tcp(msg);
  if (strcmp(msg, "STOP") == 0) {
    tcp_ip6_complete = 1;
    return 1;
  }
  return 0;
}
void *test_tcp_listen_ipv6(void *args) {
  tcp_socket_listener_callback c = tcplistenipv6_callback;
  tcp_ipv6_listener = jnx_socket_tcp_create(AF_INET6);
  jnx_socket_tcp_listen(tcp_ipv6_listener,TESTPORT1,100,c);
  return 0;
}
void setup_tcp_listen_ipv6_test() {
  JNX_LOG(NULL,"Starting TCP IPV6 test");
  pthread_t worker;
  pthread_create(&worker,NULL,test_tcp_listen_ipv6,NULL);

  sleep(2);
  //listener setup lets send a message
  jnx_socket *s = jnx_socket_tcp_create(AF_INET6);
  size_t bytes = jnx_socket_tcp_send(s,"::1",TESTPORT1,"hi",strlen("hi"));
  printf("."); fflush(stdout); sleep(1);
  bytes = jnx_socket_tcp_send(s,"::1",TESTPORT1,"bye",strlen("bye"));
  printf("."); fflush(stdout); sleep(1);
  bytes = jnx_socket_tcp_send(s,"::1",TESTPORT1,"once more for good measure",strlen("once more for good measure"));
  printf("."); fflush(stdout); sleep(1);
  bytes = jnx_socket_tcp_send(s,"::1",TESTPORT1,"STOP",strlen("STOP"));
  printf(".\n");
  jnx_socket_destroy(&s);
  time_t st;
  time(&st);
  int max = 5;
  float elapsed = 0;
  while(tcp_ip6_complete == 0 && elapsed < max) {

    time_t e;
    time(&e);
    elapsed += (e-st);
  }
  jnx_socket_destroy(&tcp_ipv6_listener);
}


static int udp_ip4_complete = 0;
jnx_socket *udp_ipv4_listener;
static int udp_ip6_complete = 0;
jnx_socket *udp_ipv6_listener;
/////////////////////UDP IPV4 /////////////////////////////
int udplistenipv4_callback(char *msg, size_t size,jnx_socket *s) {
  printf("udplisten_callback => %s\n",msg);
  assert(strcmp(msg,"hi") == 0);
  udp_ip4_complete = 1;
  return 1;
}
void *test_udp_listen_ipv4(void *args) {
  udp_socket_listener_callback c = udplistenipv4_callback;
  udp_ipv4_listener = jnx_socket_udp_create(AF_INET);
  jnx_socket_udp_listen(udp_ipv4_listener,TESTPORT2,100,c);
  return 0;
}
void setup_udp_listen_ipv4_test() {
  JNX_LOG(NULL,"Starting UDP IPV4 test");
  pthread_t worker;
  pthread_create(&worker,NULL,test_udp_listen_ipv4,NULL);

  sleep(2);
  //listener setup lets send a message
  jnx_socket *s = jnx_socket_udp_create(AF_INET);
  size_t bytes = jnx_socket_udp_send(s,"127.0.0.1",TESTPORT2,"hi",strlen("hi"));
  jnx_socket_destroy(&s);
  time_t st;
  time(&st);
  int max = 5;
  float elapsed = 0;
  while(udp_ip4_complete == 0 && elapsed < max) {

    time_t e;
    time(&e);
    elapsed += (e-st);
  }
  jnx_socket_destroy(&udp_ipv4_listener);
}
/////////////////////UDP IPV6 /////////////////////////////
int  udplistenipv6_callback(char *msg, size_t size,jnx_socket *s) {
  assert(strcmp(msg,"hi") == 0);
  udp_ip6_complete = 1;
  return 1;
}
void *test_udp_listen_ipv6(void *args) {
  udp_socket_listener_callback c = udplistenipv6_callback;
  udp_ipv6_listener = jnx_socket_udp_create(AF_INET6);
  jnx_socket_udp_listen(udp_ipv6_listener,TESTPORT3,100,c);
  return 0;
}
void setup_udp_listen_ipv6_test() {
  JNX_LOG(NULL,"Starting UDP IPV6 test");
  pthread_t worker;
  pthread_create(&worker,NULL,test_udp_listen_ipv6,NULL);

  sleep(2);
  //listener setup lets send a message
  jnx_socket *s = jnx_socket_udp_create(AF_INET6);
  size_t bytes = jnx_socket_udp_send(s,"::1",TESTPORT3,"hi",strlen("hi"));
  jnx_socket_destroy(&s);

  time_t st;
  time(&st);
  int max = 5;
  float elapsed = 0;
  while(udp_ip6_complete == 0 && elapsed < max) {

    time_t e;
    time(&e);
    elapsed += (e-st);
  }
  jnx_socket_destroy(&udp_ipv6_listener);
}
////////////////TEST RECEIPT (TCP)///////////////////////
jnx_uint32 test_receipt_ipv4_listener_callback(jnx_uint8 *msg,jnx_size bytes,
    jnx_socket *s, int connected_sockfd) {

  write(connected_sockfd,"Hello",6);
  return 1;
}
void *bootstrap_listener(void *args) {
  jnx_socket *listener = (jnx_socket *)args;
  jnx_socket_tcp_listen(listener,TESTPORT4,1,test_receipt_ipv4_listener_callback);

}
void test_receipt_ipv4() {
  JNX_LOG(NULL,"Starting test_receipt IPV4");
  jnx_socket *listener = jnx_socket_tcp_create(AF_INET);
  jnx_thread_create_disposable(bootstrap_listener,listener);
  
  sleep(1);
  
  jnx_socket *sender = jnx_socket_tcp_create(AF_INET);
  jnx_char *received;
  jnx_size bytes_read = jnx_socket_tcp_send_with_receipt(sender,
      "127.0.0.1",TESTPORT4,"Something",10,&received);

  JNXCHECK(strcmp(received,"Hello") == 0);

}
int main(int argc, char **argv) {
  JNX_LOG(NULL,"Starting network tests\n");
  test_create();
  setup_tcp_listen_ipv4_test();
  setup_tcp_listen_ipv6_test();

  setup_udp_listen_ipv4_test();
  setup_udp_listen_ipv6_test();

  test_receipt_ipv4();
  return 0;
}
