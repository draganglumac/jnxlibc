/*
 * =====================================================================================
 *
 *       Filename:  jnxthread.h
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
#if !defined(WIN32)
#include <pthread.h>
#else
#include <windows.h>
#endif
typedef struct jnxthread_attributes{
	//platform specific zone//
	int has_custom_attr;
#if !defined(WIN32)
	pthread_attr_t *system_attributes;
#endif
	//platform specific zone//
}jnxthread_attributes;

typedef void *(entry_point)(void*);

typedef struct jnxthread{
	int id;
	void *args;
	jnxthread_attributes *attributes;
	//platform specific zone//
#if !defined(WIN32)
	pthread_t system_thread;
#else
	HANDLE system_thread;
#endif
	//platform specific zone//
}jnxthread;

typedef union jnxthread_mutex{
	//platform specific zone//
#if !defined(WIN32)
	pthread_mutex_t system_mutex;
#endif
	//platform specific zone//
}jnxthread_mutex;


int jnxthread_unlock(jnxthread_mutex *m);
/**
 *@fn jnxthread_lock(jnxthread_mutex *m)
 *@brief blocking function that will wait for the mutex to unlock
 *@param jnxthread_mutex to lock
 */
void jnxthread_lock(jnxthread_mutex *m);
/**
 *@fn int jnxthread_trylock
 *@brief tries to lock a mutex, returns error code on failure or 0 on success
 *@param jnxthread_mutex mutex to lock
 *@return int errorcode 0 on success
 */
int jnxthread_trylock(jnxthread_mutex *m);
/**
 *@fn void jnxthread_poolflush
 *@brief resets the pool and removes thread data
 *@warning Does not ensure threads have terminated
 */
void jnxthread_poolflush();
/**
 *@fn size_t jnxthread_poolcount
 *@brief counts the current number of threads in the pool
 *@warning does not give thread status active/inactive
 *@return size_t of thread count
 */
size_t jnxthread_poolcount();
/**
 *@fn jnxthread* jnxthread_create(entry_point e,void *args)
 *@param entry_point is the function pointer the thread starts with
 *@param args are the arguments to pass the function pointer
 *@brief jnxthread_create will create and start a new thread, adding to the pool
 *@return jnxthread object
 */
jnxthread* jnxthread_create(entry_point e,void *args);
/**
 *@fn void jnxthread_destroy(jnxthread *thr)
 *@brief Destroy the thread data structure and pool listing
 *@warning Destroy will not ensure thread is killed
 */
void jnxthread_destroy(jnxthread *thr);
/**
 *@fn int jnxthread_join(jnxthread *thr)
 *@param jnxthread pointer to thread object to wait for
 *@param data the exit data from the target thread
 *@brief passing a jnxthread pointer will wait for that thread to complete before unblocking
 *@return the ret code from join execution
 */
int jnxthread_join(jnxthread *thr, void **data);

#ifdef __cplusplus
	}
#endif
#endif
