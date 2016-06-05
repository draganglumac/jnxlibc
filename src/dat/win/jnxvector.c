/*
 * =====================================================================================
 *
 *       Filename:  jnx_vector.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/16/13 14:32:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "jnxlog.h"
#include "jnxvector.h"
#include "jnxcheck.h"
jnx_vector *jnx_vector_create(void) {
  jnx_vector *vector = malloc(sizeof(jnx_vector));
  vector->count = 0;
  vector->vector = NULL;
  vector->internal_lock = jnx_thread_mutex_create();
  return vector;
}
jnx_vector_record *jnx_vector_record_create(void *value) {
  jnx_vector_record *record = malloc(sizeof(record));
  record->data = value;
  record->used = 1;

  return record;
}
jnx_vector_record *jnx_vector_record_create_empty() {
  jnx_vector_record *record = malloc(sizeof(record));
  record->data = NULL;
  record->used = 0;
  return record;
}
//One of the primary reasons not to free data here is we don't know where it belongs too
//and it may cause a segfault if its on the stack
void jnx_vector_destroy(jnx_vector** vector) {
  JNXCHECK(*vector);
  jnx_int32 x = 0;
  for ( x = 0; x < (*vector)->count; ++x ) {
    free((*vector)->vector[x]);
  }
  jnx_thread_mutex_destroy(&(*vector)->internal_lock);
  free((*vector)->vector);
  free(*vector);
}
void jnx_vector_grow(jnx_vector **vector, jnx_int32 increment) {
  jnx_int32 resize = (*vector)->count + increment;
  jnx_vector_record **temp = realloc((*vector)->vector,resize * sizeof(jnx_vector_record));
  if(temp == NULL) {
    JNXLOG(LDEBUG,"Error with reallocing\n");
    exit(0);
  } else {
    (*vector)->vector = temp;
  }
}
void jnx_vector_fillspace(jnx_vector **vector,jnx_int32 start, int32_t end) {
  while(start < end) {
    (*vector)->vector[start] = jnx_vector_record_create_empty();
    ++start;
  }
}
void jnx_vector_insert_at(jnx_vector *vector, jnx_int32 position, void *value) {
  JNXCHECK(vector);
  JNXCHECK(value);
  if(position > vector->count) {
    jnx_int32 different = position - vector->count;
    jnx_vector_grow(&vector,different);
    jnx_vector_fillspace(&vector,vector->count,vector->count + different + 1);
    vector->count = vector->count + different;
    vector->vector[vector->count]->data = value;
    vector->vector[vector->count]->used = 1;
    vector->count++;
  } else {
    vector->vector[position]->used = 1;
    vector->vector[position]->data = value;
  }
}
void jnx_vector_insert_at_ts(jnx_vector *vector, jnx_int32 position, void *value) {
  JNXCHECK(vector);
  JNXCHECK(value);
  jnx_thread_lock(vector->internal_lock);
  jnx_vector_insert_at(vector,position,value);
  jnx_thread_unlock(vector->internal_lock);
}
void* jnx_vector_get_at(jnx_vector *vector,jnx_int32 position) {
  JNXCHECK(vector);
  if(vector->vector[position]->used) {
    void *data = vector->vector[position]->data;
    return data;
  }
  return NULL;
}
void* jnx_vector_get_at_ts(jnx_vector *vector,jnx_int32 position) {
  JNXCHECK(vector);
  jnx_thread_lock(vector->internal_lock);
  void *ret = jnx_vector_get_at(vector,position);
  jnx_thread_unlock(vector->internal_lock);
  return ret;
}
void* jnx_vector_remove_at(jnx_vector *vector,jnx_int32 position) {
  JNXCHECK(vector);
  if(vector->vector[position]->used) {
    void *data = vector->vector[position]->data;
    vector->vector[position]->data = NULL;
    vector->vector[position]->used = 0;
    return data;
  }
  return NULL;
}
void* jnx_vector_remove_at_ts(jnx_vector *vector,jnx_int32 position) {
  JNXCHECK(vector);
  jnx_thread_lock(vector->internal_lock);
  void *ret = jnx_vector_remove_at(vector,position);
  jnx_thread_unlock(vector->internal_lock);
  return ret;
}
void jnx_vector_insert(jnx_vector *vector, void *value) {
  JNXCHECK(vector);
  jnx_vector_record *record = jnx_vector_record_create(value);
  jnx_vector_grow(&vector,1);
  vector->vector[vector->count] = record;
  vector->count++;
}
void jnx_vector_insert_ts(jnx_vector *vector, void *value) {
  JNXCHECK(vector);
  jnx_thread_lock(vector->internal_lock);
  jnx_vector_insert(vector,value);
  jnx_thread_unlock(vector->internal_lock);
}
void *jnx_vector_last(jnx_vector *vector) {
  JNXCHECK(vector);
  if(vector->count == 0) {
    JNXLOG(LDEBUG,"Cannot pop from an empty vector\n");
    return NULL;
  }
  void *data = vector->vector[vector->count -1 ]->data;
  free(vector->vector[vector->count -1]);
  vector->count--;
  vector->vector = realloc(vector->vector, (vector->count) * sizeof(void*));
  return data;
}
void *jnx_vector_last_ts(jnx_vector *vector) {
  JNXCHECK(vector);
  jnx_thread_lock(vector->internal_lock);
  void *ret = jnx_vector_last(vector);
  jnx_thread_unlock(vector->internal_lock);
  return ret;
}
jnx_size jnx_vector_count(jnx_vector *vector) {
  JNXCHECK(vector);
  return vector->count;
}
jnx_size jnx_vector_count_ts(jnx_vector *vector) {
  JNXCHECK(vector);
  jnx_thread_lock(vector->internal_lock);
  jnx_size ret = jnx_vector_count(vector);
  jnx_thread_unlock(vector->internal_lock);
  return ret;
}
jnx_int32 jnx_vector_contains(jnx_vector *vector, void *datain, int32_t(*vector_comparison)(void *a,void *b)) {
  jnx_int32 x = 0;
  for ( x = 0; x < vector->count; ++x ) {
    if(vector_comparison(vector->vector[x]->data,datain)){
      return 1;
    }
  }
  return 0;
}
jnx_int32 jnx_vector_contains_ts(jnx_vector *vector, void *datain, int32_t(*vector_comparison)(void *a,void *b)) {
  jnx_thread_lock(vector->internal_lock);
  jnx_int32 f = jnx_vector_contains(vector,datain,vector_comparison);
  jnx_thread_unlock(vector->internal_lock);
  return f;
}
