/*
 * =====================================================================================
 *
 *       Filename:  jnxmem.c
 *
 *    Description:  
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
static jnx_list *memtrace = NULL;

void jnx_mem_memtrace(char *path)
{
#ifdef __JNX_DEBUG_MEMORY_UNMANAGED__
	return;
#endif
	if(memtrace == NULL) return ;
	jnx_node *h = memtrace->head;
	char *state_al = "[IN USE]";
	char *state_fr = "[FREE]";
	while(h)
	{
		jnx_mem_memtrace_item *m = h->_data;
		char str[1024];
		char *t;
		switch(m->state)
		{
			case FREE:
				t = state_fr;
				break;
			case ALLOC:
				t = state_al;
				break;
		}	
		sprintf(str,"[%s][%s:%d][%p][size:%zu] - %s\n",m->file,m->function,m->line,m->ptr,m->size,t);
		jnx_file_write(path,str,strlen(str),"a");
		h = h->next_node;
	}
	size_t total_bytes = jnx_mem_memtrace_get_byte_alloc();
	size_t total_allocs = jnx_mem_memtrace_get_total_number_alloc();
	char buffer[1024];
	time_t t;
	char *buf;
	time(&t);
	buf = (char*)malloc(strlen(ctime(&t)) +1);
	snprintf(buf,strlen(ctime(&t)),"%s",ctime(&t));
	char *debug ="Time:%d\nTotal allocs:%zu\nCurrent allocs:%zu\nTotal bytes:%zu(%zuKb)\n";
	sprintf(buffer,debug,buf,total_allocs,jnx_mem_memtrace_get_current_number_alloc(),total_bytes,(total_bytes / 1024));
	free(buf);	
	jnx_file_write(path,buffer,strlen(buffer),"a");
}
size_t jnx_mem_memtrace_clear_memory()
{
	if(!memtrace)
	{
		printf("Warning memtrace list is empty\n");
		return 0;
	}
	size_t clear_mem = 0;
	jnx_node *m = memtrace->head;
	while(m)
	{
		jnx_mem_memtrace_item *mi = m->_data;
		if(mi->state == ALLOC)
		{
			free(mi->ptr);
			free(mi->function);
			free(mi->file);
			clear_mem = clear_mem + mi->size;
			mi->state = FREE;
		}
		free(m->_data);
		m = m->next_node;
	}
	jnx_list_delete(&memtrace);
	memtrace = NULL;
	return clear_mem;
}
size_t jnx_mem_memtrace_get_total_number_alloc()
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
size_t jnx_mem_memtrace_get_current_number_alloc()
{
	if(memtrace == NULL)
	{
		return 0;
	}
	size_t ta = 0;
	jnx_node *h = memtrace->head;
	while(h)
	{
		jnx_mem_memtrace_item *m = h->_data;
		if(m->state == ALLOC){
			++ta;
		}
		h = h->next_node;
	}
	return ta;
}
size_t jnx_mem_memtrace_get_byte_alloc()
{
	if(memtrace == NULL)
	{
		return 0;
	}
	size_t tb = 0;
	jnx_node *h = memtrace->head;
	while(h)
	{
		jnx_mem_memtrace_item *m = h->_data;
		if(m->state == ALLOC){
			tb += m->size;
		}
		h = h->next_node;
	}
	return tb;
}
jnx_list *jnx_mem_memtrace_get_list()
{
	if(memtrace == NULL)
	{
		return NULL;
	}
	return memtrace;
}
static void jnx_mem_new_alloc(void *ptr, size_t size,char* file,const char *function,int line)
{
	jnx_mem_memtrace_item *m = malloc(sizeof(jnx_mem_memtrace_item));
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
	if(memtrace == NULL)
	{
		memtrace = jnx_list_init(); 
	}
	jnx_list_add(memtrace,m);
}
void* jnx_mem_malloc(size_t size,char *file,const char *function,int line)
{
	void *p = malloc(size);	
#ifndef __JNX_DEBUG_MEMORY_UNMANAGED__
	jnx_mem_new_alloc(p,size,file,function,line);
#endif
	return p;
}
void* jnx_mem_calloc(size_t num,size_t size,char *file,const char *function,int line)
{
	void *p = calloc(num,size);
#ifndef __JNX_DEBUG_MEMORY_UNMANAGED__
	jnx_mem_new_alloc(p,size,file,function,line);
#endif
	return p;
}
static void adjust_state_in_list(void *ptr)
{
	jnx_node *h = memtrace->head;
	while(h)
	{
		jnx_mem_memtrace_item *m = h->_data;
		if(m->ptr == ptr)
		{
			m->state = FREE;
		}
		h = h->next_node;
	}
}
void* jnx_mem_realloc(void *ptr,size_t size,char *file,const char *function,int line)
{

#ifndef __JNX_DEBUG_MEMORY_UNMANAGED__
	adjust_state_in_list(ptr);
#endif
	void *p = realloc(ptr,size);
#ifndef __JNX_DEBUG_MEMORY_UNMANAGED__
	jnx_mem_new_alloc(p,size,file,function,line);
#endif
	return p;
}
void jnx_mem_free(void *ptr)
{
#ifndef __JNX_DEBUG_MEMORY_UNMANAGED__
	adjust_state_in_list(ptr);	
#endif
	free(ptr);
}
