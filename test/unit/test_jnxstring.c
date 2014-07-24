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
int main(int args, char **argv) {
	JNX_LOGC(JLOG_DEBUG,"Running test for jnxstring\n");

	JNX_LOGC(JLOG_DEBUG,"- test_string_join");
	char *temp = malloc(256);
	strcpy(temp,"foo");
	jnx_string_join(&temp,"bar");
	assert(strcmp(temp,"foobar") == 0);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");

	int a = 15;
	char *expected_output="15";
	char *actual_output = jnx_string_itos(a);
	assert(strcmp(expected_output,actual_output) == 0);
	JNX_LOGC(JLOG_DEBUG,"String tests completed\n");
	return 0;
}
