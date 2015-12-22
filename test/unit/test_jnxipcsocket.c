#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnx_ipc_socket.h"
#include "jnxthread.h"
#include <assert.h>
#include <time.h>

#define SOCKPATH "cotton_socks"

static int test_ipc_listener_complete = 0;

static void *worker(void *args) {
  int id = (int)args;
  jnx_ipc_socket *t = jnx_socket_ipc_create(SOCKPATH);
  char buffer[8];
  sprintf(buffer, "ping %d", id);
  jnx_socket_ipc_send(t,buffer,strlen(buffer));

  return NULL;
}
static void fire_threaded_ipc_packet(int id) {
  jnx_thread_create_disposable(worker,(void *)id);
}

static void test_ipc_listener_callback(const jnx_uint8 *payload,
                                       jnx_size bytes_read,
                                       int connected_socket, void *args){
  JNXCHECK(bytes_read == 8);
  JNXCHECK(strcmp((char *)payload, "ping 69") == 0);
  test_tcp_listener_complete = 1;
}
static void test_ipc_listener() {
  jnx_ipc_listener *listener =
    jnx_socket_ipc_listener_create(SOCKPATH, 100);

  fire_threaded_ipc_packet(69);
  int x = 0;
  while(x < 5) {
    jnx_socket_tcp_listener_tick(listener,test_ipc_listener_callback,NULL);
    if(test_ipc_listener_complete)break;
    ++x;
  }
  jnx_socket_ipc_listener_destroy(&listener);
  JNXCHECK(test_ipc_listener_complete);
  JNXCHECK(listener == NULL);
}
static void test_blocking_listener_callback(const jnx_uint8 *payload,
                                            jnx_size bytes_read,
                                            int connected_socket, void *args){
  JNXCHECK(bytes_read == 8);
  JNXCHECK(strcmp((char *)payload, "ping 96") == 0);
  test_tcp_listener_complete = 1;
}
static void *worker_blocking_listener(void *args) {
  jnx_tcp_listener **listener = args;
  *listener = jnx_socket_tcp_listener_create(SOCKPATH, 100);
  jnx_socket_tcp_listener_auto_tick(*listener,test_blocking_listener_callback,
      NULL);

  return NULL;
}
static void test_ipc_blocking_listener() {
  jnx_tcp_listener *listener;

  jnx_thread_create_disposable(worker_blocking_listener,&listener);
  sleep(1);
  fire_threaded_ipc_packet(SOCKPATH, 96);
  clock_t start = clock(),diff;
  jnx_float msec = 0;
  while(msec < 500 && !test_tcp_listener_complete){
    diff = clock();
    msec = (diff - start) * 1000 / CLOCKS_PER_SEC;
  }
  listener->hint_exit = 1;
  JNXCHECK(test_ipc_listener_complete);
}
int test_jnxipcsocket() {
  JNXLOG(LDEBUG,"Starting IPC socket tests");
  JNXLOG(LDEBUG,"Testing IPC Listener");
  test_ipc_listener();
  test_ipc_listener_complete = 0;
  JNXLOG(LDEBUG,"Test IPC blocking listener");
  test_ipc_blocking_listener();
  return 0;
}
