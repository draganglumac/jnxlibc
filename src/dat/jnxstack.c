#include <stdlib.h>
#include "jnxstack.h"
jnx_stack* jnx_stack_create(void) {
    jnx_stack *s = malloc(sizeof(jnx_stack));
    s->count = 0;
    s->top = NULL;

    return s;
}
int jnx_stack_is_empty(jnx_stack* A) {
    if ( A->top == NULL && A->count == 0 ) {
        return 1;
    }

    return 0;
}
int jnx_stack_is_empty_ts(jnx_stack* A) {
	jnx_thread_lock(&A->internal_lock);
	int ret = jnx_stack_is_empty(A);
	jnx_thread_unlock(&A->internal_lock);
	return ret;
}
void jnx_stack_push(jnx_stack* A, void* _datain) {
    if ( _datain == NULL ) {
        // We don't accept NULL data
        return;
    }
    jnx_snode *temp = malloc(sizeof(jnx_snode));
    temp->_data = _datain;
    temp->next_node = A->top;
    A->top = temp;
    A->count++;
}
void jnx_stack_push_ts(jnx_stack* A, void* _datain) {
	jnx_thread_lock(&A->internal_lock);
	jnx_stack_push(A,_datain);
	jnx_thread_unlock(&A->internal_lock);
}
void* jnx_stack_pop(jnx_stack* A) {
    if ( A->top == NULL ) {
        return NULL;
    }

    jnx_snode *temp = A->top;
    void *retval = temp->_data;
    A->top = temp->next_node;
    free(temp);
    A->count--;

    return retval;
}
void* jnx_stack_pop_ts(jnx_stack* A) {
	jnx_thread_lock(&A->internal_lock);
	void *ret = jnx_stack_pop(A);
	jnx_thread_unlock(&A->internal_lock);
	return ret;
}
void jnx_stack_destroy(jnx_stack** A) {
    while ( (*A)->top != NULL ) {
        jnx_snode *temp = (*A)->top;
        (*A)->top = temp->next_node;
        free(temp);
        (*A)->count--;
    }
    *A = NULL;
}
