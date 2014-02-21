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
#include <assert.h>
#include "jnxlog.h"
#include <time.h>
#include <string.h>
#include "jnxterm.h"

char *A = "TestA";
char *B = "TestB";
char *C = "TestC";
char *ar[] = { "A","B","C"};
void test_stack_grow()
{
	JNX_LOGC("- test_stack_grow");
	jnx_stack *s = jnx_stack_create();
	assert(s);
	jnx_stack_push(s,A);
	jnx_stack_push(s,B);
	jnx_stack_push(s,C);
	jnx_stack_destroy(&s);
	assert(s == NULL);
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_stack_pop()
{
	JNX_LOGC("- test_stack_pop");
	jnx_stack *s = jnx_stack_create();
	assert(s);
	jnx_stack_push(s,ar[0]);
	jnx_stack_push(s,ar[1]);
	jnx_stack_push(s,ar[2]);

	int c = 2;	
	while(!jnx_stack_is_empty(s))
	{
		char *d = jnx_stack_pop(s);
		assert(strcmp(ar[c],d) == 0);
		--c;
	}	
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
int main(int argc, char **argv)
{
    JNX_LOGC("Running stack tests...\n");
	test_stack_grow();
	test_stack_pop();
    JNX_LOGC("Stack tests completed.\n");
    return 0;
}
