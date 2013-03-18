/*
 * =====================================================================================
 *
 *       Filename:  jnx_vector.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/16/13 11:43:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __JNX_VECTOR_H__
#define __JNX_VECTOR_H__
typedef struct{
    void *data;
}jnx_vector_record;
typedef struct{
    int count;
    jnx_vector_record **vector;
}jnx_vector;
jnx_vector *jnx_vector_init(void);
//The user is responsible for deleting pointers contained within the vector
//Delete will only remove the vector structure and its datastructure components
void jnx_vector_delete(jnx_vector* vector);
void jnx_vector_push(jnx_vector *vector, void *value);
void jnx_vector_insert(jnx_vector *vector, int position, void *value);
//returns the last element in the vector and removes it from the array
void *jnx_vector_pop(jnx_vector *vector);
#endif 
