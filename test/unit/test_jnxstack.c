/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/16/13 11:42:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxstack.h"
#include <stdio.h>
#include "jnxcheck.h"
#include "jnxlog.h"
#include <time.h>
#include <string.h>
#include "jnxterm.h"

char *A = "TestA";
char *B = "TestB";
char *C = "TestC";
char *ar[] = { "A","B","C"};
void test_stack_grow() {
  JNXLOG(LDEBUG,"- test_stack_grow");
  jnx_stack *s = jnx_stack_create();
  JNXCHECK(s);
  jnx_stack_push(s,A);
  jnx_stack_push(s,B);
  jnx_stack_push(s,C);
  jnx_stack_destroy(&s);
  JNXCHECK(s == NULL);
  JNXLOG(LDEBUG,"OK");
}
void test_stack_pop() {
  JNXLOG(LDEBUG,"- test_stack_pop");
  jnx_stack *s = jnx_stack_create();
  JNXCHECK(s);
  jnx_stack_push(s,ar[0]);
  jnx_stack_push(s,ar[1]);
  jnx_stack_push(s,ar[2]);

  int c = 2;
  while(!jnx_stack_is_empty(s)) {
    char *d = jnx_stack_pop(s);
    JNXCHECK(strcmp(ar[c],d) == 0);
    --c;
  }
  JNXLOG(LDEBUG,"OK");
}
int compare_func(void *a, void *b) {
  if(a == b) {
    return 1;
  }
  return 0;
}
void test_contains(){
  JNXLOG(LDEBUG,"- test_stack_contains");
  jnx_stack *s = jnx_stack_create();
  jnx_stack_push(s,ar[0]);
  JNXCHECK(jnx_stack_contains(s,ar[0],compare_func) == 1);
  JNXCHECK(jnx_stack_contains(s,ar[1],compare_func) == 0);
  JNXLOG(LDEBUG,"OK");
}
int test_jnxstack() {
  JNXLOG(LDEBUG,"Running stack tests...\n");
  test_stack_grow();
  test_stack_pop();
  test_contains();
  JNXLOG(LDEBUG,"Stack tests completed.\n");
  return 0;
}
