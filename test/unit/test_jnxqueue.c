/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/27/14 13:59:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxqueue.h"
#include "jnxlog.h"
#include "jnxterm.h"
#include <assert.h>
#include <stdio.h>
int test_jnxqueue() {

  JNXLOG(LDEBUG,"Running Queue tests:");	
  jnx_queue *q = jnx_queue_create();

  char *ar[5] = { "A", "B", "C", "D", "E" };

  int x;
  for(x = 0; x < 5;++x) {
    jnx_queue_push(q,ar[x]);
  }
  for(x=5 ;x > 0; --x) {
    char *r = jnx_queue_pop(q);
    assert(jnx_queue_count(q) == (x-1));
  }

  jnx_queue_destroy(&q);
  JNXLOG(LDEBUG,"OK");

  JNXLOG(LDEBUG,"Queue tests completed\n");
  return 0;
}
