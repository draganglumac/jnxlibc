/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/01/14 16:47:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include "jnxsignal.h"
#include "jnxlog.h"
#include "jnxthread.h"
int local_slot_tickA =0;
int local_slot_tickB =0;
void local_slot(jnx_signal *s) {
  assert(s);
  int *d = (int*)s->data;
  if(d) {
    if(*d == 5) {
      ++local_slot_tickA;
    }else {
      ++local_slot_tickB;
    }
  }	
}
void local_slot2(jnx_signal *s) {
}
void simple_test() {

  JNXLOG(LDEBUG,"Simple signal slot test\n");

  jnx_signal *s = jnx_signal_create();
  jnx_signal_connect(s,local_slot);
  jnx_signal_connect(s,local_slot2);
  jnx_signal_call(s,NULL);
  jnx_signal_destroy(&s);
  assert(s == NULL);
}

void *simple_threaded_worker(void *args) {
  jnx_signal *y = args;
  int duration = 2.0f;
  time_t s,e;
  time(&s);
  double diff = 0;
  int _b = 2;
  int *B = &_b;
  while(diff < duration) {
    jnx_signal_call(y,B);
    time(&e);
    diff = difftime(e,s); 
  }
  return 0;
}
void simple_threaded_test() {

  JNXLOG(LDEBUG,"Simple thread signal slot test\n");
  jnx_signal *s = jnx_signal_create();
  jnx_signal *y = jnx_signal_create();

  jnx_signal_connect(s,local_slot);	
  jnx_signal_connect(y,local_slot);

  int _a = 5;
  int *A = &_a;
  jnx_thread_create_disposable(simple_threaded_worker,y);
  int duration = 2;
  time_t st,e;
  time(&st);
  double diff = 0;
  while(diff < duration) {
    jnx_signal_call(s,A);
    time(&e);
    diff = difftime(e,st); 
  }
  JNXLOG(LDEBUG,"ThreadA ticked slot %d times, ThreadB ticked slot %d times\n",local_slot_tickA,local_slot_tickB);

  jnx_signal_destroy(&s);
  jnx_signal_destroy(&y);
}
int dual_slot0_cb_hit = 0;
int dual_slot1_cb_hit = 0;
void dual_slot_test_callback0(jnx_signal *s) {
  dual_slot0_cb_hit = 1;
}
void dual_slot_test_callback1(jnx_signal *s) {
  dual_slot1_cb_hit = 1;
}
void dual_slot_test() {

  JNXLOG(LDEBUG,"Dual slot tests\n");
  jnx_signal *s = jnx_signal_create();
  jnx_signal_connect(s,dual_slot_test_callback0);
  jnx_signal_connect(s,dual_slot_test_callback1);
  jnx_signal_call(s,NULL);
  assert(dual_slot0_cb_hit == 1);
  assert(dual_slot1_cb_hit == 1);
  jnx_signal_destroy(&s);
}
int cross_thread_A = 0;
int cross_thread_B = 0;

void cross_thread_slot_callback(jnx_signal *s) {
  int *d = (int*)s->data;
  if(*d == 5) {
    cross_thread_A = 1;
  }else {
    cross_thread_B = 1;
  }
}
int cross_thread_lock_try = 0;
void *cross_thread_worker(void *args) {
  jnx_signal *s = args;
  int _a = 4;
  int *A = &_a;
  int ret = jnx_signal_call(s,A);
  if(ret) {
    JNXLOG(LDEBUG,"ThreadB unable to lock: %d\n",ret);
  }
  cross_thread_lock_try = 1;
  return 0;
}
void cross_thread_slot_test() {
  JNXLOG(LDEBUG,"Cross thread slot test\n");

  jnx_signal *s = jnx_signal_create();
  jnx_signal_connect(s,cross_thread_slot_callback);

  int _a = 5;
  int *A = &_a;
  jnx_thread_create_disposable(cross_thread_worker,s);
  int ret = jnx_signal_call(s,A);
  if(ret) {
    JNXLOG(LDEBUG,"TheadA unable to lock: %d\n",ret);
  }
  if(ret == 0) {
    assert(cross_thread_A == 1);
  }else {
    assert(cross_thread_B == 1);
  }

}
int test_jnxsignal() {

  JNXLOG(LDEBUG,"Testing signals and slots\n");	
  simple_test(); //one thread

  simple_threaded_test();

  dual_slot_test();

  cross_thread_slot_test();

  return 0;
}
