#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jnx_log.h"
#include "jnx_check.h"
#include "jnx_ipc_socket.h"
#include "jnx_thread.h"
#include <assert.h>
#include <time.h>

#define SOCKPATH "cotton_socks"

static int test_ipc_listener_complete = 0;

static void *worker(void *args) {
  int id = (int) args, i;
  jnx_ipc_socket *t = jnx_socket_ipc_create(SOCKPATH);
  char buffer[8];
  sprintf(buffer, "ping %d", id);
  for (i = 0; i < 5; i++) {
    sleep(1);
    jnx_socket_ipc_send(t, buffer, strlen(buffer));
  }
  return NULL;
}

static void fire_threaded_ipc_packets(int id) {
  jnx_thread_create_disposable(worker, (void *) id);
}

static void test_ipc_listener_callback(const jnx_uint8 *payload,
                                       jnx_size bytes_read,
                                       int connected_socket, void *args) {
  int *results = (int *) args;
  JNXCHECK(bytes_read == 7);
  if (strncmp("ping 69", (char *) payload, bytes_read) == 0)
    results[0]++;
  else if (strncmp("ping 70", (char *) payload, bytes_read) == 0)
    results[1]++;
  else if (strncmp("ping 71", (char *) payload, bytes_read) == 0)
    results[2]++;
  else {
    JNXLOG(LERROR, "Unexpected packet received");
    JNXCHECK(1 == 0);
  }
  if (results[0] == 5 && results[1] == 5 && results[2] == 5) {
    JNXLOG(LDEBUG, "All 15 packets received, 5 from each sender.");
    test_ipc_listener_complete = 1;
  }
}

static void test_ipc_listener() {
  jnx_ipc_socket *s = jnx_socket_ipc_create(SOCKPATH);
  jnx_ipc_listener *listener =
    jnx_socket_ipc_listener_create(s, 100);

  fire_threaded_ipc_packets(69);
  fire_threaded_ipc_packets(70);
  fire_threaded_ipc_packets(71);
  int results[3] = {0, 0, 0};
  while (!test_ipc_listener_complete) {
    jnx_socket_ipc_listener_tick(listener, test_ipc_listener_callback, (void *) results);
  }
  jnx_socket_ipc_listener_destroy(&listener);
  JNXCHECK(test_ipc_listener_complete);
  JNXCHECK(listener == NULL);
}

static void test_blocking_listener_callback(const jnx_uint8 *payload,
                                            jnx_size bytes_read,
                                            int connected_socket, void *args) {
  int *results = (int *) args;
  JNXCHECK(bytes_read == 7);
  if (strncmp("ping 96", (char *) payload, bytes_read) == 0)
    results[0]++;
  else if (strncmp("ping 97", (char *) payload, bytes_read) == 0)
    results[1]++;
  else if (strncmp("ping 98", (char *) payload, bytes_read) == 0)
    results[2]++;
  else {
    JNXLOG(LERROR, "Unexpected packet received");
    JNXCHECK(1 == 0);
  }
  if (results[0] == 5 && results[1] == 5 && results[2] == 5) {
    JNXLOG(LDEBUG, "All 15 packets received, 5 from each sender.");
    test_ipc_listener_complete = 1;
  }
}

static void *worker_blocking_listener(void *args) {
  jnx_ipc_listener **listener = args;
  jnx_ipc_socket *s = jnx_socket_ipc_create(SOCKPATH);
  *listener = jnx_socket_ipc_listener_create(s, 100);
  int results[3] = {0, 0, 0};
  while (!test_ipc_listener_complete)
    jnx_socket_ipc_listener_auto_tick(*listener,
                                      test_blocking_listener_callback,
                                      (void *) results);

  return NULL;
}

static void test_ipc_blocking_listener() {
  jnx_ipc_listener *listener;

  fire_threaded_ipc_packets(96);
  fire_threaded_ipc_packets(97);
  fire_threaded_ipc_packets(98);
  jnx_thread_create_disposable(worker_blocking_listener, &listener);
  clock_t start = clock(), diff;
  jnx_float msec = 0;
  while (!test_ipc_listener_complete) {
    sleep(1);
  }
  listener->hint_exit = 1;
  JNXCHECK(test_ipc_listener_complete);
}

int test_jnxipcsocket() {
  JNXLOG(LDEBUG, "Starting IPC socket tests");
  remove(SOCKPATH);
  JNXLOG(LDEBUG, "Testing IPC Listener");
  test_ipc_listener();
  test_ipc_listener_complete = 0;
  JNXLOG(LDEBUG, "Test IPC blocking listener");
  test_ipc_blocking_listener();
  return 0;
}
