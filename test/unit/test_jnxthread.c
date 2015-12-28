/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/12/14 18:24:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxterm.h"
#include "jnxthread.h"
#include "jnxlog.h"
#include "jnxqueue.h"
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
static int testscomplete = 0;

static jnx_queue *queue = NULL;
int counter = 0;
int stop = 0;
void *worker(void *args) {
  jnx_queue_push_ts(queue,"DATA");
  ++counter;
  return 0;
}
static void *noise(void *args) {

  while(!stop) {
    jnx_queue_pop_ts(queue);
  }
}
static void test_queue_push() {
  queue = jnx_queue_create();

  int y = 5;
  int x;
  double current_time = 0;
  float max_time = 5.0f;
  time_t s,e;
  time(&s);

  jnx_thread_create_disposable(noise,NULL);

  for(x=0;x<y;++x) {
    jnx_thread_create_disposable(worker,NULL);
    time(&e);
    current_time = difftime(e,s);
  }
  stop = 1;
  while(counter < y && current_time < max_time) {
  }
  assert(current_time < max_time);
  JNXLOG(LDEBUG,"Queue length %d\n",queue->list->counter);
}
int test_jnxthread() {

  JNXLOG(LDEBUG,"Running jnx_threading tests\n");
  test_queue_push();

  return 0;
}
