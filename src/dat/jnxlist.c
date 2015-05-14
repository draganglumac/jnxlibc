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
 *         Author:  jonesax (jonesax@hush.com),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxlist.h"
#include "jnxlog.h"
#include "jnxcheck.h"
#include <stdio.h>

jnx_list* jnx_list_create(void) {
  jnx_list *list = malloc(sizeof(jnx_list));
  list->head = NULL;
  list->tail = NULL;
  list->counter = 0;
  list->internal_lock = jnx_thread_mutex_create();
  return list;
}
void jnx_list_add(jnx_list* A, void* _datain) {
  JNXCHECK(A);
  JNXCHECK(_datain);

  jnx_node *node = malloc(sizeof(jnx_node));
  node->_data = _datain;
  node->next_node = NULL;
  node->prev_node = NULL;

  jnx_node *current = A->head;
  if (current == NULL) {
    A->head = node;
  }
  else {
    while (current->next_node != NULL)
      current = current->next_node;
    current->next_node = node;
    node->prev_node = current;
  }
  A->tail = node;
  A->counter++;
  return;
}
void jnx_list_add_ts(jnx_list* A, void* _datain) {
  jnx_thread_lock(A->internal_lock);
  jnx_list_add(A,_datain);
  jnx_thread_unlock(A->internal_lock);
}
void* jnx_list_remove(jnx_list** A) {
  if((*A)->head == NULL) {
    return NULL;
  }
  jnx_node *last = (*A)->tail;
  jnx_node *penultimate = last->prev_node;
  (*A)->tail = penultimate;
  if (penultimate == NULL) {
    (*A)->head = NULL;
  }
  else {
    penultimate->next_node = NULL;
  }
  void *data = last->_data;
  free(last);
  (*A)->counter--;
  return data;
}
void* jnx_list_remove_ts(jnx_list** A) {
  jnx_list *l = *A;
  jnx_thread_lock(l->internal_lock);
  void *ret = jnx_list_remove(A);
  jnx_thread_unlock(l->internal_lock);
  return ret;
}

void *jnx_list_remove_front(jnx_list ** A) {
  if((*A)->head == NULL) {
    return NULL;
  }
  jnx_node *first = (*A)->head;
  jnx_node *second = first->next_node;
  (*A)->head = second;
  if (second == NULL) {
    (*A)->tail = NULL;
  }
  else {
    second->prev_node = NULL;
  }
  void *data = first->_data; 
  free(first);
  (*A)->counter--;
  return data;
}
void *jnx_list_remove_front_ts(jnx_list ** A) {
  jnx_list *l = *A;
  jnx_thread_lock(l->internal_lock);
  void *ret = jnx_list_remove_front(A);
  jnx_thread_unlock(l->internal_lock);
  return ret;
}
jnx_size jnx_list_count(jnx_list *A) {
  jnx_size count = A->counter;
  return count;
}
jnx_size jnx_list_count_ts(jnx_list *A) {
  jnx_thread_lock(A->internal_lock);
  jnx_size count = jnx_list_count(A);
  jnx_thread_unlock(A->internal_lock);
  return count;
}
jnx_int32 jnx_list_contains(jnx_list *A, void *datain, jnx_int32 (*list_comparison)(void *a,void *b) ){
  JNXCHECK(A);
  JNXCHECK(datain);
  JNXCHECK(list_comparison);
  jnx_node *current = A->head;
  jnx_int32 f=0; 
  while(current) {
    if(list_comparison(datain,current->_data)) {
      f = 1;
      break;
    }
    current = current->next_node;
  }
  return f;
}
jnx_int32 jnx_list_contains_ts(jnx_list *A, void *datain, jnx_int32 (*list_comparison)(void *a,void *b) ){
  jnx_thread_lock(A->internal_lock);
  jnx_int32 f = jnx_list_contains(A,datain,list_comparison);
  jnx_thread_unlock(A->internal_lock);
  return f;
}
void jnx_list_destroy(jnx_list** A) {
  if((*A) == NULL) {
    JNXLOG(LDEBUG,"jnx_list_destroy: No list\n");
    return;
  }
  if((*A)->head == NULL) {
    (*A) = NULL;
    return;
  }
  jnx_node *current = (*A)->head;
  if(!current->next_node) {

    free(current);
    free(*A);
    (*A) = NULL;
    return;
  }
  while(current) {
    jnx_node *current_node = current;
    current = current->next_node;
    free(current_node);
  }
  jnx_thread_mutex_destroy(&(*A)->internal_lock);
  free(*A);
  (*A) = NULL;
}

