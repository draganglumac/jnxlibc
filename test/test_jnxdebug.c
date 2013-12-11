/*
 * =====================================================================================
 *
 *       Filename:  test_jnxdebug.c
 *
 *    Description:  Memory manager tests
 *
 *        Version:  1.0
 *        Created:  12/06/13 08:22:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesaax@hush.com
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
	printf("- test_allocation test  ");
	void *d = jnx_debug_malloc(sizeof(char) *30);

	jnx_list *l = jnx_debug_memtrace_get_list();
	jnx_debug_memtrace_item *m = jnx_list_remove(&l);

	assert(m->ptr == d);
	jnx_debug_memtrace_clear_memory();
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_allocation_long()
{
	printf("- test_allocation_long test  ");
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
	printf("- test_list ");
	assert(jnx_debug_memtrace_get_list() == NULL);
	void *p = jnx_debug_calloc(2,sizeof(char) * 3);
	jnx_debug_memtrace_clear_memory();
	assert(jnx_debug_memtrace_get_list() == NULL);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_large()
{
	printf("- test_large ");
	
	int size = 10000;
	int i;
	size_t a = sizeof(char) * 100;
	for(i=0;i<size;++i)
	{
		
		char *s = jnx_debug_malloc(a);
		assert(jnx_debug_memtrace_get_byte_alloc() == ((i+1) * a));
		assert(jnx_debug_memtrace_get_alloc() == (i + 1));
	}
	size_t b = jnx_debug_memtrace_clear_memory();
	assert(jnx_debug_memtrace_get_alloc() == 0);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");

}
void test_deallocation()
{
	printf("- test_deallocation ");
	assert(jnx_debug_memtrace_get_alloc() == 0);	

	int x = 100;
	int c;
	int o = 0;
	size_t s = (sizeof(int));
	size_t st = s * 100;
	int *A = jnx_debug_malloc(sizeof(int));
	long int aptr = *A;
	jnx_debug_free(&A);
	assert(jnx_debug_memtrace_get_alloc() == 0);	
	assert(jnx_debug_memtrace_get_byte_alloc() == 0);
	assert(A == NULL);
	for(c=0;c<x;++c)
	{
		int *B = jnx_debug_malloc(s);
	}
	size_t bytes = jnx_debug_memtrace_clear_memory();
	assert(bytes == st);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
int main(int argc, char **argv)
{
	printf("Running memory management tests...\n");
	test_allocation();
	test_allocation_long();
	test_list();
	test_large();
	test_deallocation();
	return 0;
}
