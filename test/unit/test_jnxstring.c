/*
 * =====================================================================================
 *
 *       Filename:  test_jnxhash.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/27/13 16:20:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "jnxlog.h"
#include "jnxstring.h"
#include "jnxterm.h"
int test_jnxstring() {
  JNXLOG(LDEBUG,"Running test for jnxstring\n");

  JNXLOG(LDEBUG,"- test_string_join");
  char *temp = malloc(256);
  strcpy(temp,"foo");
  jnx_string_join(&temp,"bar");
  assert(strcmp(temp,"foobar") == 0);
  JNXLOG(LDEBUG,"OK");

  int a = 15;
  char *expected_output="15";
  char *actual_output = jnx_string_itos(a);
  assert(strcmp(expected_output,actual_output) == 0);
  JNXLOG(LDEBUG,"String tests completed\n");
  return 0;
}
