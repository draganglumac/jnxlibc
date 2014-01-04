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

typedef struct mem_node{
	void *data;
	struct mem_node *next;
}mem_node;

typedef struct mem_list{
	struct mem_node *head;
	int counter;
}mem_list;

static mem_list *memtrace = NULL;

void add_link(mem_list *m,void *in)
{
	if(m->head == NULL)
	{
		mem_node *n = malloc(sizeof(mem_node));
		n->data = in;
		n->next = NULL;
		m->head = n;
		m->counter++;
		return;
	}
	mem_node *org_head = m->head;
	while(m->head)
	{
		mem_node *current = m->head;
		if(!m->head->next)
		{
			mem_node *n = malloc(sizeof(mem_node));
			n->data = in;
			n->next = NULL;
			current->next = n;
			m->counter++;
			m->head = org_head;
			return;
		}
		m->head = m->head->next;
	}	
	m->head = org_head;
}
mem_list *init_mem_list()
{
	mem_list *m = malloc(sizeof(mem_list));
	m->counter = 0;
	m->head = NULL;
	return m;
}
void list_delete(mem_list **m)
{
	if((*m) == NULL) { return; }
	if((*m)->head == NULL){ return; }
	mem_node *current = (*m)->head;
	if(!current->next)
	{
		free(current);
		free(*m);
		(*m) = NULL;
		return;
	}
	while(current)
	{
		mem_node *cn = current;
		free(cn);
		current = current->next;
	}
	free(*m);
	(m) = NULL;
}
void jnx_mem_memtrace(char *path)
{
#ifdef RELEASE
	return;
#endif
	if(memtrace == NULL) return ;
	mem_node *h = memtrace->head;
	char *state_al = "[IN USE]";
	char *state_fr = "[FREE]";
	while(h)
	{
		jnx_mem_memtrace_item *m = h->data;
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
		h = h->next;
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
	mem_node *m = memtrace->head;
	while(m)
	{
		jnx_mem_memtrace_item *mi = m->data;
		if(mi->state == ALLOC)
		{
			free(mi->ptr);
			free(mi->function);
			free(mi->file);
			clear_mem = clear_mem + mi->size;
			mi->state = FREE;
		}
		free(m->data);
		m = m->next;
	}
	list_delete(&memtrace);
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
	mem_node *h = memtrace->head;
	while(h)
	{
		++ta;
		h = h->next;
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
	mem_node *h = memtrace->head;
	while(h)
	{
		jnx_mem_memtrace_item *m = h->data;
		if(m->state == ALLOC){
			++ta;
		}
		h = h->next;
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
	mem_node *h = memtrace->head;
	while(h)
	{
		jnx_mem_memtrace_item *m = h->data;
		if(m->state == ALLOC){
			tb += m->size;
		}
		h = h->next;
	}
	return tb;
}
mem_list *jnx_mem_memtrace_get_list()
{
	if(memtrace == NULL)
	{
		return NULL;
	}
	return memtrace; 
}
static inline void jnx_mem_new_alloc(void *ptr, size_t size,char* file,const char *function,int line)
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
		memtrace = init_mem_list(); 
	}
	add_link(memtrace,m);
}
void* jnx_mem_malloc(size_t size,char *file,const char *function,int line)
{
	void *p = malloc(size);	
#ifndef RELEASE
	jnx_mem_new_alloc(p,size,file,function,line);
#endif
	return p;
}
void* jnx_mem_calloc(size_t num,size_t size,char *file,const char *function,int line)
{
	void *p = calloc(num,size);
#ifndef RELEASE
	jnx_mem_new_alloc(p,size,file,function,line);
#endif
	return p;
}
static void adjust_state_in_list(void *ptr)
{
	mem_node *h = memtrace->head;
	while(h)
	{
		jnx_mem_memtrace_item *m = h->data;
		if(m->ptr == ptr)
		{
			m->state = FREE;
		}
		h = h->next;
	}
}
void* jnx_mem_realloc(void *ptr,size_t size,char *file,const char *function,int line)
{
#ifndef RELEASE
	adjust_state_in_list(ptr);
#endif
	void *p = realloc(ptr,size);
#ifndef RELEASE
	jnx_mem_new_alloc(p,size,file,function,line);
#endif
	return p;
}
void jnx_mem_free(void *ptr)
{
#ifndef RELEASE
	adjust_state_in_list(ptr);	
#endif
	free(ptr);
}
