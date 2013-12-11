/*
 * =====================================================================================
 *
 *       Filename:  jnxdebug.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/27/13 18:59:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxdebug.h"
#include "jnxfile.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <execinfo.h>
#include <string.h>
typedef void (*jnx_debug_internal_handler)(int);
static char *write_path = NULL;
static jnx_list *memtrace = NULL;
void jnx_debug_stacktrace_cb(int s)
{
	void *array[MAX_TRACE];
	size_t size;
	char **str;
	size_t i;
	size = backtrace(array,MAX_TRACE);
	str=backtrace_symbols(array,size);
	for(i = 0; i < size; ++i)
	{
		printf("[%zu]%s\n",i,str[i]);
		int len = strlen(str[i]);
		str[i][len] = '\n';
		jnx_file_write(write_path,str[i],strlen(str[i]),"a");
	}
	free(str);
	exit(1);
}
void jnx_debug_stacktrace(char *path, int counter,int signals[])
{
	write_path = path;
	assert(write_path);	
	jnx_debug_internal_handler c = jnx_debug_stacktrace_cb;
	int i;
	for(i=0;i<counter;++i)
	{
		signal(signals[i],c);
	}
}
void jnx_debug_memtrace(char *path)
{
	if(memtrace == NULL) return ;
	jnx_node *h = memtrace->head;
	while(h)
	{
		jnx_debug_memtrace_item *m = h->_data;
		char str[1024];
		sprintf(str,"[%s][%p][size:%zu]\n",__FILE__,m->ptr,m->size);
	    jnx_file_write(path,str,strlen(str),"a");
		h = h->next_node;
	}
	size_t total_bytes = jnx_debug_memtrace_get_byte_alloc();
	size_t total_allocs = jnx_debug_memtrace_get_alloc();
	char buffer[1024];
    time_t t;
    char *buf;
    time(&t);
    buf = (char*)malloc(strlen(ctime(&t)) +1);
    snprintf(buf,strlen(ctime(&t)),"%s",ctime(&t));
	char *debug ="Time:%d\nTotal allocs:%zu\nTotal bytes:%zu\n";
	sprintf(buffer,debug,buf,total_allocs,total_bytes);
	free(buf);	
	jnx_file_write(path,buffer,strlen(buffer),"a");
}
size_t jnx_debug_memtrace_clear_memory()
{
	if(!memtrace) return 0;
	size_t clear_mem = 0;
	jnx_node *m = memtrace->head;
	while(m)
	{
		jnx_debug_memtrace_item *mi = m->_data;
		clear_mem += mi->size;
		free(mi->ptr);
		free(m->_data);
		m = m->next_node;
	}
	jnx_list_delete(&memtrace);
	memtrace = NULL;
	return clear_mem;
}
size_t jnx_debug_memtrace_get_alloc()
{
	if(memtrace == NULL)
	{
		return 0;
	}
	size_t ta = 0;
	jnx_node *h = memtrace->head;
	while(h)
	{
		++ta;
		h = h->next_node;
	}
	return ta;
}
size_t jnx_debug_memtrace_get_byte_alloc()
{
	if(memtrace == NULL)
	{
		return 0;
	}
	size_t tb = 0;
	jnx_node *h = memtrace->head;
	while(h)
	{
		jnx_debug_memtrace_item *m = h->_data;
		tb += m->size;
		h = h->next_node;
	}
	return tb;
}
jnx_list *jnx_debug_memtrace_get_list()
{
	if(memtrace == NULL)
	{
		return NULL;
	}
	return memtrace;
}
static void jnx_debug_new_alloc(void *ptr, size_t size)
{
	jnx_debug_memtrace_item *m = malloc(sizeof(jnx_debug_memtrace_item));
	m->ptr = ptr;
	m->size = size;
	if(memtrace == NULL)
	{
		memtrace = jnx_list_init(); 
	}
	jnx_list_add(memtrace,m);
}
void* jnx_debug_malloc(size_t size)
{
	void *p = malloc(size);	
	jnx_debug_new_alloc(p,size);
	return p;
}
void* jnx_debug_calloc(size_t num,size_t size)
{
	void *p = calloc(num,size);
	jnx_debug_new_alloc(p,size);
	return p;
}
static void remove_from_list(void *ptr)
{
	jnx_node *h = memtrace->head;
	jnx_list *s = jnx_list_init();

	while(h)
	{
		jnx_debug_memtrace_item *m = h->_data;

		if(m->ptr == ptr)
		{
			free(m);
		}else{
			jnx_list_add(s,m);
		}
		h = h->next_node;
	}
	jnx_list_delete(&memtrace);
	memtrace = s;
}
void* jnx_debug_realloc(void *ptr,size_t size)
{
	remove_from_list(ptr);
	void *p = realloc(ptr,size);
	jnx_debug_new_alloc(p,size);
	return p;
}
void jnx_debug_free(void **ptr)
{
	remove_from_list(*ptr);	
	free(*ptr);
	*ptr = NULL;
}
