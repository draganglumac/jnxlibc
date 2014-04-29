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
void *worker(void *args) {
	jnx_queue_push_ts(queue,"DATA");
	++counter;
	return 0;
}
void test_queue_push() {
	queue = jnx_queue_create();

	int y = 5;
	int x;
	double current_time = 0;
	float max_time = 5.0f;
	time_t s,e;
	time(&s);
	for(x=0;x<y;++x) {
		jnx_thread_create_disposable(worker,NULL);
		time(&e);
		current_time = difftime(e,s);
	}
	while(counter < y && current_time < max_time) {
	}
	assert(current_time < max_time);
	JNX_LOGC(JLOG_DEBUG,"Queue length %d\n",queue->list->counter);
}
int main(int argc, char **argv) {

	JNX_LOGC(JLOG_DEBUG,"Running jnx_threading tests\n");
	test_queue_push();




    return 0;
}
