/*
 * =====================================================================================
 *
 *       Filename:  jnxmem.c
 *
 *    Description: Memory manager that uses internal datastructures for tracking 
 *
 *        Version:  1.0
 *        Created:  01/03/14 18:16:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxmem.h"
#include "jnxbtree.h"
#include "jnxlist.h"
typedef enum { FREE, ALLOC }jnx_mem_memtrace_state;
typedef struct{
	void *ptr;
	size_t size;
	char *file;
	char *function;
	int line;
	jnx_mem_memtrace_state state;

}jnx_mem_item;


int internal_compare_callback(void *A, void *B)
{
	if(A > B)
	{
		return -1;
	}
	if(B > A)
	{
		return 1;
	}
	return 0;
}
static jnx_btree *memtree = NULL;
void jnx_mem_clear()
{
	if(memtree == NULL)
	{
		return;
	}
	jnx_list *l = jnx_list_create();
	jnx_btree_keys(memtree,l);	
	while(l->head)
	{
		jnx_mem_item *m = jnx_btree_lookup(memtree,l->head->_data);
		if(m)
		{
				free(m->function);
				free(m->file);
			if(m->state == ALLOC)
			{
				free(m->ptr);
			}
			free(m);
			l->head = l->head->next_node;
		}
	}
	jnx_btree_destroy(memtree);
	memtree = NULL;
}	
size_t jnx_mem_get_current_size_allocations()
{
	if(memtree == NULL)
	{
		return 0;
	}
	jnx_list *l = jnx_list_create();
	jnx_btree_keys(memtree,l);	
	size_t n = 0;
	while(l->head)
	{
		jnx_mem_item *m = jnx_btree_lookup(memtree,l->head->_data);
		if(m)
		{
			if(m->state == ALLOC)
			{
				n = n + m->size;
			}
			l->head = l->head->next_node;
		}
	}
	jnx_list_destroy(&l);
	return n;
}
size_t jnx_mem_get_current_number_allocations()
{
	if(memtree == NULL)
	{
		return 0;
	}
	jnx_list *l = jnx_list_create();
	jnx_btree_keys(memtree,l);	
	size_t n = 0;
	while(l->head)
	{
		jnx_mem_item *m = jnx_btree_lookup(memtree,l->head->_data);
		if(m)
		{
			if(m->state == ALLOC)
			{
				++n;
			}
			l->head = l->head->next_node;
		}
	}
	jnx_list_destroy(&l);
	return n;
}
size_t jnx_mem_get_total_size_allocations()
{
	if(memtree == NULL)
	{
		return 0;
	}
	jnx_list *l = jnx_list_create();
	jnx_btree_keys(memtree,l);	
	size_t n = 0;
	while(l->head)
	{
		jnx_mem_item *m = jnx_btree_lookup(memtree,l->head->_data);
		if(m)
		{
			n = n + m->size;
			l->head = l->head->next_node;
		}
	}
	jnx_list_destroy(&l);
	return n;
}	
size_t jnx_mem_get_total_number_allocations()
{
	if(memtree == NULL)
	{
		return 0;
	}
	jnx_list *l = jnx_list_create();
	jnx_btree_keys(memtree,l);	
	size_t n = 0;
	n = l->counter;	
	jnx_list_destroy(&l);
	return n;
}
static inline void jnx_mem_new(void *ptr, size_t size,char* file,const char *function,int line)
{
	jnx_mem_item *m = malloc(sizeof(jnx_mem_item));
	if(m == NULL)
	{
		printf("Error with allocation\n [%zu(kb)]",(size /1024));
		return;
	}
	m->ptr = ptr;
	m->size = size;
	m->state = ALLOC;
	m->file = strdup(file);
	m->function = strdup(function);
	m->line = line;

	if(memtree == NULL)
	{
		compare_keys c = internal_compare_callback;
		memtree = jnx_btree_create(sizeof(int),c);
	}
	jnx_btree_add(memtree,ptr,m);
}
void* jnx_mem_malloc(size_t size,char *file,const char *function,int line)
{
	void *p = malloc(size);	
	jnx_mem_new(p,size,file,function,line);
	return p;
}
void* jnx_mem_calloc(size_t num,size_t size,char *file,const char *function,int line)
{
	void *p = calloc(num,size);
	jnx_mem_new(p,size,file,function,line);
	return p;
}
void* jnx_mem_realloc(void *ptr,size_t size,char *file,const char *function,int line)
{
	void *p = realloc(ptr,size);
	jnx_mem_new(p,size,file,function,line);
	return p;
}
void jnx_mem_free(void *ptr)
{
	jnx_mem_item *m = jnx_btree_lookup(memtree,ptr);
	if(m)
	{
		m->state = FREE;
	}
	free(ptr);
}
