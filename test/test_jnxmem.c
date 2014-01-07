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
#include "jnxterm.h"
#include "jnxlist.h"
#include "jnxmem.h"
#include <assert.h>


//Here we are accessing functions not publicly accessible
typedef struct mem_node{
	void *data;
	struct mem_node *next;
}mem_node;

typedef struct mem_list{
	struct mem_node *head;
	int counter;
}mem_list;

extern mem_list *jnx_mem_get_list();
//
void print_mem()
{
	mem_list *l = jnx_mem_get_list();
	mem_node *h = l->head;
	while(h)
	{
		jnx_mem_item *m = h->data;
		printf("[%x][size: %d]\n",m->ptr,m->size);
		h = h->next;
	}
}
void test_allocation()
{
	printf("- test_allocation test  ");
	void *d = JNX_MEM_MALLOC(sizeof(char) *30);

	mem_list *l = jnx_mem_get_list();
	jnx_mem_item *m = l->head->data;

	assert(m->ptr == d);
	jnx_mem_clear();
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_allocation_long()
{
	printf("- test_allocation_long test  ");
	int x;
	size_t total_mem = 0;
	for(x=0;x<1000;++x)
	{
		char *s = JNX_MEM_MALLOC(sizeof(char) * x);
		total_mem += (sizeof(char) * x);
	}
	mem_node *h = jnx_mem_get_list()->head;
	size_t out_mem = 0;
	while(h)
	{
		jnx_mem_item *b = h->data;
		out_mem += b->size;
		h = h->next;
	}
	assert(out_mem == total_mem);
	size_t b = jnx_mem_clear();
	assert(b == out_mem);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_list()
{
	printf("- test_list ");
	assert(jnx_mem_get_list() == NULL);
	void *p = JNX_MEM_CALLOC(2,sizeof(char) * 3);
	jnx_mem_clear();
	assert(jnx_mem_get_list() == NULL);
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

		char *s = JNX_MEM_MALLOC(a);
		assert(jnx_mem_get_byte_alloc() == ((i+1) * a));
		assert(jnx_mem_get_total_number_alloc() == (i + 1));
	}
	size_t b = jnx_mem_clear();
	assert(jnx_mem_get_current_number_alloc() == 0);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");

}
void test_deallocation()
{
	printf("- test_deallocation ");
	assert(jnx_mem_get_total_number_alloc() == 0);	

	int x = 100;
	int c;
	int o = 0;
	size_t s = (sizeof(int));
	size_t st = s * 100;
	int *A = JNX_MEM_MALLOC(sizeof(int));
	long int aptr = *A;
	jnx_mem_free(A);
	assert(jnx_mem_get_current_number_alloc() == 0);	
	assert(jnx_mem_get_byte_alloc() == 0);
	for(c=0;c<x;++c)
	{
		int *B = JNX_MEM_MALLOC(s);
	}
	size_t bytes = jnx_mem_clear();
	assert(bytes == st);
	assert(jnx_mem_get_current_number_alloc() == 0);	
	assert(jnx_mem_get_byte_alloc() == 0);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");

}
void test_malloc_dealloc_balance()
{
	printf("- test malloc dealloc balance");
	size_t onemg = 1024 *1024 * 1024;
	size_t onegig = (onemg * 1024);
	jnx_list *p = jnx_list_init();
	int counter = 0;
	while(jnx_mem_get_byte_alloc() < onegig)
	{
		char *d = JNX_MEM_MALLOC(onemg);
		jnx_list_add(p,d);
		++counter;
	}	

	while(p->head)
	{
		JNX_MEM_FREE(p->head->_data);
		p->head = p->head->next_node;
	}

	jnx_list_delete(&p);
	assert(counter == 1024);

	jnx_mem_clear();
	assert(jnx_mem_get_current_number_alloc() == 0);
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
	test_malloc_dealloc_balance();
	return 0;
}
