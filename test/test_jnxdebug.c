/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/06/13 08:22:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include "../src/jnxterm.h"
#include "../src/jnxdebug.h"
#include <assert.h>
void print_mem()
{
	jnx_node *h = jnx_debug_memtrace_get_list()->head;
	while(h)
	{
		jnx_debug_memtrace_item *m = h->_data;
		printf("[%x][size: %d]\n",m->ptr,m->size);
		h = h->next_node;
	}
}
void test_allocation()
{
	printf("Running test_allocation test - ");
	void *d = jnx_debug_malloc(sizeof(char) *30);

	jnx_list *l = jnx_debug_memtrace_get_list();
	jnx_debug_memtrace_item *m = jnx_list_remove(&l);

	assert(m->ptr == d);
	jnx_debug_memtrace_clear_memory();
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_allocation_long()
{
	printf("Running test_allocation_long test - ");
	int x;
	size_t total_mem = 0;
	for(x=0;x<1000;++x)
	{
		char *s = jnx_debug_malloc(sizeof(char) * x);
		total_mem += (sizeof(char) * x);
	}
	jnx_node *h = jnx_debug_memtrace_get_list()->head;
	size_t out_mem = 0;
	while(h)
	{
		jnx_debug_memtrace_item *b = h->_data;
		out_mem += b->size;
		h = h->next_node;
	}
	assert(out_mem == total_mem);
	size_t b = jnx_debug_memtrace_clear_memory();
	assert(b == out_mem);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_list()
{
	printf("Running test_list - ");
	assert(jnx_debug_memtrace_get_list() == NULL);
	void *p = jnx_debug_calloc(2,sizeof(char) * 3);
	jnx_debug_memtrace_clear_memory();
	assert(jnx_debug_memtrace_get_list() == NULL);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
int main(int argc, char **argv)
{
	printf("Running jnx_debug tests...\n");
	test_allocation();
	test_allocation_long();
	test_list();
	printf("jnx_debug tests completed\n");
	return 0;
}
