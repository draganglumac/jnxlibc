#include <stdio.h>
#include <stdlib.h>
#include "jnx_log.h"
#include "jnx_check.h"
#include "jnx_udp_socket.h"
#include "jnx_thread.h"
#include <assert.h>
#include "jnx_thread.h"
#include <time.h>
#include <unistd.h>
#include "jnx_network.h"
#define UDPTESTPORT "9093"
#define UDPTESTPORT2 "9041"
#define BGROUP "225.1.1.1"
static int test_udp_listener_complete = 0;
// Broadcast address IPv4
static struct sockaddr *filter_broadcast_address(struct ifaddrs *addr) {
  struct sockaddr *ip = addr->ifa_addr;
  struct sockaddr *netmask = addr->ifa_netmask;
  ((struct sockaddr_in *) ip)->sin_addr.s_addr |= ~(((struct sockaddr_in *) netmask)->sin_addr.s_addr);
  return ip;
}
// Local IP address IPv4
static struct sockaddr *filter_local_ip_address(struct ifaddrs *addr) {
  struct sockaddr *ip = addr->ifa_addr;
  return ip;
}
static void *worker(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_udp_create(AF_INET);
  jnx_socket_udp_send(t,"127.0.0.1",port,(jnx_uint8*)"ping",5);
  return NULL;
}
static void *worker_ipv6(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_udp_create(AF_INET6);
  jnx_socket_udp_send(t,"::1",port,(jnx_uint8*)"ping",5);
  return NULL;
}
static void *worker_broadcast(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_udp_create(AF_INET);
  JNXLOG(LDEBUG,"worker_broadcast firing...");
  jnx_socket_udp_broadcast_send(t,"255.255.255.255",port,(jnx_uint8*)"ping",5);
  return NULL;
}
static void *worker_multicast(void *args) {
  char *port = (char*)args;
  jnx_socket *t = jnx_socket_udp_create(AF_INET);
  jnx_socket_udp_multicast_send(t,BGROUP,port,(jnx_uint8*)"ping",5);
  return NULL;
}
static void fire_threaded_udp_packet(char *port) {
  jnx_thread_create_disposable(worker,port);
}
static void fire_threaded_udp_packet_ipv6(char *port) {
  jnx_thread_create_disposable(worker_ipv6,port);
}
static void fire_threaded_udp_packet_broadcast(char *port) {
  jnx_thread_create_disposable(worker_broadcast,port);
}
static void fire_threaded_udp_packet_multicast(char *port) {
  jnx_thread_create_disposable(worker_multicast,port);
}
static void test_udp_listener_callback(const jnx_uint8 *payload, \
    jnx_size bytes_read, void *args){
  JNXLOG(LDEBUG,"test_udp_listener_callback achieved");
  test_udp_listener_complete = 1;
}
static void test_udp_listener() {
  jnx_udp_listener *listener = 
    jnx_socket_udp_listener_create(UDPTESTPORT,AF_INET,NULL);

  fire_threaded_udp_packet(UDPTESTPORT);
  int x = 0;
  while(x < 20) {
    jnx_socket_udp_listener_tick(listener,test_udp_listener_callback,NULL);
    if(test_udp_listener_complete)break;
    sleep(.5);
    ++x;
  }
  jnx_socket_udp_listener_destroy(&listener);
  JNXCHECK(test_udp_listener_complete);
  JNXCHECK(listener == NULL);
}
static void test_udp_listener_ipv6() {
  jnx_udp_listener *listener = 
    jnx_socket_udp_listener_create(UDPTESTPORT,AF_INET6,NULL);
  fire_threaded_udp_packet_ipv6(UDPTESTPORT);
  int x = 0;
  while(x < 20) {
    jnx_socket_udp_listener_tick(listener,test_udp_listener_callback,NULL);
    if(test_udp_listener_complete)break;
    sleep(.5);
    ++x;
  }
  jnx_socket_udp_listener_destroy(&listener);
  JNXCHECK(test_udp_listener_complete);
  JNXCHECK(listener == NULL);
}

static void test_udp_broadcast(){
  jnx_udp_listener *listener = 

    jnx_socket_udp_listener_broadcast_create(UDPTESTPORT,AF_INET,NULL);
  JNXLOG(LDEBUG,"Firing broadcast packet");
  fire_threaded_udp_packet_broadcast(UDPTESTPORT);
  int x = 0;
  while(x < 20) {
    JNXLOG(LDEBUG,"Ticking jnx_socket_udp_listener_tick");
    jnx_socket_udp_listener_tick(listener,test_udp_listener_callback,NULL);
    if(test_udp_listener_complete)break;
    sleep(.5);
    ++x;
  }
  jnx_socket_udp_listener_destroy(&listener);
  JNXCHECK(test_udp_listener_complete);
  JNXCHECK(listener == NULL);
}
static void test_blocking_listener_callback(const jnx_uint8 *payload, \
    jnx_size bytes_read, void *args){
  test_udp_listener_complete = 1;
}
static void *worker_blocking_listener(void *args) {
  jnx_udp_listener **listener = args;
  jnx_socket_udp_listener_auto_tick(*listener,test_blocking_listener_callback,
      NULL);
  return NULL;
}
static void test_udp_blocking_listener() {
  jnx_udp_listener *listener = jnx_socket_udp_listener_create(UDPTESTPORT,
      AF_INET,NULL);

  jnx_thread_create_disposable(worker_blocking_listener,&listener);
  sleep(1);
  fire_threaded_udp_packet(UDPTESTPORT);
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
static void test_udp_multicast(){
  char *ip;
  jnx_network_interface_ip(&ip,NULL,AF_INET);

  JNXLOG(LDEBUG,"Using IP => %s",ip);
  jnx_udp_listener *listener = 
    jnx_socket_udp_listener_multicast_create(UDPTESTPORT2,AF_INET,ip,BGROUP,NULL);

  fire_threaded_udp_packet_multicast(UDPTESTPORT2);
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
int test_jnxudpsocket() {
  JNXLOG(LDEBUG,"Starting udp socket tests");
  JNXLOG(LDEBUG,"Testing UDP listener");
  test_udp_listener();
  test_udp_listener_complete = 0;
#ifndef TEST_DISABLE_IPV6
  JNXLOG(LDEBUG,"Testing UDP listener IPV6");
  test_udp_listener_ipv6();
  test_udp_listener_complete = 0;
#endif
  // JNXLOG(LDEBUG,"Testing UDP broadcast");
  // test_udp_broadcast(); 
  // test_udp_listener_complete = 0;
  // JNXLOG(LDEBUG,"Test UDP blocking listener");
  // test_udp_blocking_listener();
  // test_udp_listener_complete = 0;
  // JNXLOG(LDEBUG,"Test UDP multicast listener");
  // test_udp_multicast();
  return 0;
}
