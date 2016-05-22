/*
 * =====================================================================================
 *
 *       Filename:  jnx_thread.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/12/14 08:14:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxthread.h"
#include "jnxlog.h"
#include "jnxcheck.h"

jnx_thread_mutex* jnx_thread_mutex_create() {
  jnx_thread_mutex *m = malloc(sizeof(jnx_thread_mutex));
  m->system_mutex = CreateMutex(NULL, FALSE, NULL);
  m->is_initialized = 1;
  return m;
}
void jnx_thread_mutex_destroy(jnx_thread_mutex **m) {
  /*JNXCHECK(*m);
  if((*m)->is_initialized) {	
    pthread_mutex_destroy(&(*m)->system_mutex);
    (*m)->is_initialized = 0;
  }*/
  free(*m);
  *m = NULL;
}
jnx_int32 jnx_thread_unlock(jnx_thread_mutex *m) {

  ReleaseMutex(m->system_mutex);
  return 0;
}
jnx_int32 jnx_thread_trylock(jnx_thread_mutex *m) {
  JNXCHECK(m);
  jnx_int32 ret = 0;
  
  jnx_thread_lock(m);

  return ret;
}
void jnx_thread_lock(jnx_thread_mutex *m) {
  JNXCHECK(m);
  
  WaitForSingleObject(m->system_mutex, INFINITE);

}
void jnx_thread_handle_destroy(jnx_thread *thr) {
  /*JNXCHECK(thr);
  if(thr == NULL) {
    return;
  }
  if(thr->attributes->has_custom_attr) {
    pthread_attr_destroy(thr->attributes->system_attributes);
  }
  free(thr->attributes);*/
  CloseHandle(thr->system_thread);
  free(thr);
}
jnx_thread* jnx_thread_create(LPTHREAD_START_ROUTINE e,void *args) {
  jnx_thread *thr = malloc(sizeof(jnx_thread));
  jnx_thread_attributes *attr = malloc(sizeof(jnx_thread_attributes));
  attr->has_custom_attr = 0;
  thr->attributes = attr;
  //platform specific zone//

  thr->system_thread = CreateThread(
	  NULL,                   // default security attributes
	  0,                      // use default stack size  
	  e,       // thread function name
	  args,          // argument to thread function 
	  0,                      // use default creation flags 
	  NULL);   // returns the thread identifier 

  //platform specific zone//
  return thr;
}
jnx_int32 jnx_thread_create_disposable(entry_point e,void *args) {
  jnx_int32 ret = 0;
  //platform specific zone//
  //pthread_t disposable_thr;
  //pthread_attr_t *default_attr = NULL;
  //ret = pthread_create(&disposable_thr,default_attr,e,args);
  ////platform specific zone//
  return ret;
}
jnx_int32 jnx_thread_join(jnx_thread *thr, void **data) {
  JNXCHECK(thr);
  jnx_int32 ret = 0;
  //platform specific zone//
 // ret = pthread_join(thr->system_thread,data);
  //platform specific zone//
  return ret;
}
