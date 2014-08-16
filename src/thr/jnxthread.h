/*
 * =====================================================================================
 *
 *       Filename:  jnx_thread.h
 *
 *    Description:  A simple threading wrapper to simplify crossplatform transition
 *
 *        Version:  1.0
 *        Created:  02/12/14 08:01:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __JNX_THREAD_H__
#define __JNX_THREAD_H__
#ifdef __cplusplus
extern "C" {
#endif
#include <pthread.h>
  typedef struct jnx_thread_attributes{
    //platform specific zone//
    int has_custom_attr;
    pthread_attr_t *system_attributes;
    //platform specific zone//
  }jnx_thread_attributes;

  typedef void *(entry_point)(void*);

  typedef struct jnx_thread{
    int id;
    void *args;
    jnx_thread_attributes *attributes;
    //platform specific zone//
    pthread_t system_thread;
    //platform specific zone//
  }jnx_thread;

  typedef struct jnx_thread_mutex{
    //platform specific zone//
    pthread_mutex_t system_mutex;
    //platform specific zone//
    int is_initialized;
  }jnx_thread_mutex;


  jnx_thread_mutex* jnx_thread_mutex_create();

  void jnx_thread_mutex_destroy(jnx_thread_mutex **m);

  int jnx_thread_unlock(jnx_thread_mutex *m);
  /**
   *@fn jnx_thread_lock(jnx_thread_mutex *m)
   *@brief blocking function that will wait for the mutex to unlock
   *@param jnx_thread_mutex to lock
   */
  void jnx_thread_lock(jnx_thread_mutex *m);
  /**
   *@fn int jnx_thread_trylock
   *@brief tries to lock a mutex, returns error code on failure or 0 on success
   *@param jnx_thread_mutex mutex to lock
   *@return int errorcode 0 on success
   */
  int jnx_thread_trylock(jnx_thread_mutex *m);
  /**
   *@fn jnx_thread* jnx_thread_create(entry_point e,void *args)
   *@param entry_point is the function pointer the thread starts with
   *@param args are the arguments to pass the function pointer
   *@brief jnx_thread_create will create and start a new thread, adding to the pool
   *@return jnx_thread object
   */
  jnx_thread* jnx_thread_create(entry_point e,void *args);
  /**
   *@fn int jnx_thread_create_disposable(entry_point e,void *args)
   *@param entry_point is the function pointer the thread starts with
   *@param args are the arguments to pass the function pointer
   *@brief jnx_thread_create will create and start a new thread but does not add to pool
   *@return error code if any
   */
  int jnx_thread_create_disposable(entry_point e,void *args);
  /**
   *@fn void jnx_thread_destroy(jnx_thread *thr)
   *@brief Destroy the thread data structure and pool listing
   *@warning Destroy will not ensure thread is killed
   */
  void jnx_thread_handle_destroy(jnx_thread *thr);
  /**
   *@fn int jnx_thread_join(jnx_thread *thr)
   *@param jnx_thread pointer to thread object to wait for
   *@param data the exit data from the target thread
   *@brief passing a jnx_thread pointer will wait for that thread to complete before unblocking
   *@return the ret code from join execution
   */
  int jnx_thread_join(jnx_thread *thr, void **data);

#ifdef __cplusplus
}
#endif
#endif
