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

jnx_thread_mutex* jnx_thread_mutex_create() {
	jnx_thread_mutex *m = malloc(sizeof(jnx_thread_mutex));
	pthread_mutex_init(&m->system_mutex,NULL);	
	m->is_initialized = 1;
	return m;
}
void jnx_thread_mutex_destroy(jnx_thread_mutex **m) {
	if((*m)->is_initialized) {	
		pthread_mutex_destroy(&(*m)->system_mutex);
		(*m)->is_initialized = 0;
	}
	free(*m);
	*m = NULL;
}
int jnx_thread_unlock(jnx_thread_mutex *m) {
    int ret = 0;
    ret = pthread_mutex_unlock(&m->system_mutex);
    return ret;
}
int jnx_thread_trylock(jnx_thread_mutex *m) {
    int ret = 0;
    if(m->is_initialized != 1) {
		jnx_thread_mutex_create(m);
	}
	ret = pthread_mutex_trylock(&m->system_mutex);
    return ret;
}
void jnx_thread_lock(jnx_thread_mutex *m) {
    if(m->is_initialized != 1) {
		jnx_thread_mutex_create(m);
	}
    pthread_mutex_lock(&m->system_mutex);
}
void jnx_thread_handle_destroy(jnx_thread *thr) {
    if(thr == NULL) {
        return;
    }
    if(thr->attributes->has_custom_attr) {
        pthread_attr_destroy(thr->attributes->system_attributes);
    }
    free(thr->attributes);
    free(thr);
}
jnx_thread* jnx_thread_create(entry_point e,void *args) {
    jnx_thread *thr = malloc(sizeof(jnx_thread));
    jnx_thread_attributes *attr = malloc(sizeof(jnx_thread_attributes));
    attr->has_custom_attr = 0;
    thr->attributes = attr;
    //platform specific zone//
    pthread_attr_t *default_attr = NULL;
    pthread_create(&thr->system_thread,default_attr,e,args);
    //platform specific zone//
    return thr;
}
int jnx_thread_create_disposable(entry_point e,void *args) {
    int ret = 0;
    //platform specific zone//
    pthread_t disposable_thr;
    pthread_attr_t *default_attr = NULL;
    ret = pthread_create(&disposable_thr,default_attr,e,args);
    //platform specific zone//
    return ret;
}
int jnx_thread_join(jnx_thread *thr, void **data) {
    int ret = 0;
    //platform specific zone//
    ret = pthread_join(thr->system_thread,data);
    //platform specific zone//
    return ret;
}
