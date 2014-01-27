/*
 * =====================================================================================
 *
 *       Filename:  jnxlist.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/03/13 19:05:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxlist.h"
#include "jnxmem.h"
#include <stdio.h>

jnx_list* jnx_list_create(void)
{
	jnx_list *list = malloc(sizeof(jnx_list));
	list->head = NULL;
	list->tail = NULL;
	list->counter = 0;
	return list;
}
void jnx_list_add(jnx_list* A, void* _datain)
{
	if(A->head == NULL)
	{
		jnx_node *node = malloc(sizeof(jnx_node));
		node->_data = _datain; 
		node->next_node = NULL;
		node->prev_node = NULL;
		A->head = node;
		A->tail = node;
		A->counter++;
		return;
	}
	jnx_node *marker = A->head;
	while(A->head)
	{
		jnx_node *current_node = A->head;
		if(!A->head->next_node)
		{	
			jnx_node *node = malloc(sizeof(jnx_node));
			node->_data = _datain;
			node->next_node = NULL;
			node->prev_node = current_node;	
			current_node->next_node = node;
			A->counter++;
			A->tail = node;
			A->head = marker;
			return;
		}
		A->head = A->head->next_node;
	}
	A->head = marker;
}
void* jnx_list_remove(jnx_list** A)
{
	if((*A)->head == NULL)
	{
		return NULL;
	}
	if((*A)->head->next_node == NULL)
	{
		void *data = (*A)->head->_data;
		free((*A)->head);
		(*A)->head = NULL;
		(*A)->counter--;
		return data;
	}
	jnx_node *origin_head = (*A)->head;
	while((*A)->head)
	{
		jnx_node *next = (*A)->head->next_node;
		if(next->next_node == NULL)
		{
			void *data = next->_data;
			free(next);
			(*A)->head->next_node = NULL;
			(*A)->head = origin_head; 
			(*A)->counter--;
			return data;
		}
		(*A)->head = next;
	}
	(*A)->head = origin_head;
	return NULL;
}
void jnx_list_destroy(jnx_list** A)
{
	if((*A) == NULL)
	{
		printf("jnx_list_destroy: No list\n");
		return;
	}
	if((*A)->head == NULL)
	{
		(*A) = NULL;
		return;
	}
	jnx_node *current = (*A)->head;
	if(!current->next_node){ 

		free(current);
		free(*A);
		(*A) = NULL;
		return;
	}
	while(current)
	{
		jnx_node *current_node = current;
		current = current->next_node; 
		free(current_node);
	}
	free(*A);
	(*A) = NULL;
}

