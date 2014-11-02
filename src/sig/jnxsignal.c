/*
 * =====================================================================================
 *
 *       Filename:  jnxsignal.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/01/14 17:03:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnxsignal.h"

jnx_signal *jnx_signal_create() {
  jnx_signal *s = malloc(sizeof(jnx_signal));
  s->slot_list = jnx_list_create();
  s->internal_lock = jnx_thread_mutex_create();
  return s;
}
void jnx_signal_destroy(jnx_signal **s) {
  jnx_signal *sref = *s;
  jnx_list_destroy(&sref->slot_list);
  jnx_thread_mutex_destroy(&sref->internal_lock);
  free(sref);
  *s = NULL;	
}
void jnx_signal_connect(jnx_signal *s, jnx_slot sl) {
  JNXCHECK(s);
  jnx_thread_lock(s->internal_lock);
  jnx_list_add_ts(s->slot_list,sl);
  jnx_thread_unlock(s->internal_lock);
}
int32_t jnx_signal_call(jnx_signal *s,void *data) {
  JNXCHECK(s);
  int32_t ret = jnx_thread_trylock(s->internal_lock);   //test to see if we can lock?!
  if(ret != 0) {
    JNX_LOG(DEFAULT_CONTEXT,"Unable to lock thread for signal call\n");
    return ret;
  }
  jnx_node *h = s->slot_list->head;
  s->data = data;	
  while(s->slot_list->head) {
    jnx_slot *slot = s->slot_list->head->_data;
    slot(s);
    s->slot_list->head = s->slot_list->head->next_node;
  }
  s->slot_list->head = h;
  jnx_thread_unlock(s->internal_lock);

  return 0;
}
