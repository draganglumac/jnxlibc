/*
 * =====================================================================================
 *
 *       Filename:  jnxqueue.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/27/14 13:54:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxqueue.h"

void jnx_queue_push(jnx_queue *q, void *data) {
	if(q == NULL) {
		return;
	}
	jnx_list_add(q->list,data);
}
void jnx_queue_push_ts(jnx_queue *q, void *data) {
	jnx_thread_lock(&q->internal_lock);
	jnx_queue_push(q,data);
	jnx_thread_unlock(&q->internal_lock);
}
void *jnx_queue_pop(jnx_queue *q) {
	if(q == NULL) {
		return NULL;
	}
	if(q->list->head == NULL) {
		return NULL;
	}
	jnx_node *local = q->list->head;
	if(!local->next_node) {
		void *data = local->_data;
		q->list->head = NULL;
		q->list->counter = 0;
		return data;
	}else {
		void *data = local->_data;
		q->list->head = q->list->head->next_node;
		q->list->counter--;
		return data;
	}
	return NULL;
}
void *jnx_queue_pop_ts(jnx_queue *q) {
	jnx_thread_lock(&q->internal_lock);
	void *ret = jnx_queue_pop(q);
	jnx_thread_unlock(&q->internal_lock);
	return ret;
}
size_t jnx_queue_size(jnx_queue *q) {
	if(q == NULL) {
		return 0;
	}
	return q->list->counter;
}
size_t jnx_queue_size_ts(jnx_queue *q) {
	jnx_thread_lock(&q->internal_lock);
	size_t ret = jnx_queue_size(q);
	jnx_thread_unlock(&q->internal_lock);
	return ret;
}
jnx_queue *jnx_queue_create() {
	jnx_queue *queue = malloc(sizeof(jnx_queue));
	queue->list = jnx_list_create();
	return queue;	
}
void jnx_queue_destroy(jnx_queue **q) {
	if(*q == NULL) {
		return;
	}
	jnx_list *temp = (*q)->list;
	jnx_list_destroy(&temp);
	free(*q);
	*q = NULL;
}

