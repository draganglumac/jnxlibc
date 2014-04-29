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
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
static int testscomplete = 0;
void *worker(void *args) {
    sleep(2);
    int a =(int)args;
    return args;
}
void test_thread_add() {
    JNX_LOGC(JLOG_DEBUG,"Running test_thread_add test: ");
    jnx_thread_poolflush();
    jnx_thread *t = jnx_thread_create(worker,NULL);
    assert(jnx_thread_poolcount() == 1);
    jnx_thread *n = jnx_thread_create(worker,NULL);
    assert(jnx_thread_poolcount() == 2);
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_thread_join() {
    JNX_LOGC(JLOG_DEBUG,"Running test_thread_join test: ");
    jnx_thread_poolflush();

    time_t s,e;
    time(&s);
    jnx_thread *a = jnx_thread_create(worker,666),
                *b = jnx_thread_create(worker,NULL);
    void *data;
    int ret = jnx_thread_join(a,&data);

    time(&e);
    double duration = difftime(e,s);
    assert(data == 666);
    assert(duration == 2.0f);
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_threadpool_flush() {
    JNX_LOGC(JLOG_DEBUG,"Running test_threadpool_flush test: ");
    jnx_thread_poolflush();
    assert(jnx_thread_poolcount() == 0);
    int x;
    for(x=0; x<10; ++x) {
        jnx_thread *n = jnx_thread_create(worker,NULL);
    }
    jnx_thread_poolflush();
    assert(jnx_thread_poolcount() == 0);
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
int main(int argc, char **argv) {
    JNX_LOGC(JLOG_DEBUG,"Running jnx_threading tests\n");
    test_thread_add();
    test_threadpool_flush();
    test_thread_join();

    return 0;
}
