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
#include "jnxbtree.h"
#include "jnxlog.h"
static jnx_btree *pooltree = NULL;
static int threadcount  = 1;

int thread_id_comparison(void *A, void *B) {
    if(A > B) {
        return -1;
    }
    if(B > A) {
        return 1;
    }
    return 0;
}
void jnx_thread_poolflush() {
    if(!pooltree) {
        return;
    }
    threadcount = 1;
    jnx_list *keys = jnx_list_create();
    jnx_btree_keys(pooltree,keys);
    while(keys->head) {
        jnx_thread *thr = keys->head->_data;
        jnx_thread_destroy(thr);

        keys->head = keys->head->next_node;
    }
    jnx_list_destroy(&keys);
    jnx_btree_destroy(pooltree);
    pooltree = NULL;
}
size_t jnx_thread_poolcount() {
    if(!pooltree) {
        return 0;
    }
    jnx_list *keys = jnx_list_create();
    jnx_btree_keys(pooltree,keys);

    size_t s = keys->counter;
    jnx_list_destroy(&keys);
    return s;
}
int jnx_thread_addpool(jnx_thread *thr) {
    if(!pooltree) {
        pooltree = jnx_btree_create(sizeof(int),thread_id_comparison);
    };
    jnx_btree_add(pooltree,&thr->id,thr);
    return 0;
}
void jnx_thread_mutex_create(jnx_thread_mutex *m) {
	pthread_mutex_init(&m->system_mutex,NULL);	
	m->is_initialized = 1;
}
void jnx_thread_mutex_destroy(jnx_thread_mutex *m) {
	pthread_mutex_destroy(&m->system_mutex);
	m->is_initialized = 0;
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
void jnx_thread_destroy(jnx_thread *thr) {
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
    thr->id = threadcount++;
    //platform specific zone//
    pthread_attr_t *default_attr = NULL;
    pthread_create(&thr->system_thread,default_attr,e,args);
    //platform specific zone//
    jnx_thread_addpool(thr);
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
