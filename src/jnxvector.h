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
    int used;
    void *data;
}jnx_vector_record;
typedef struct{
    int count;
    jnx_vector_record **vector;
}jnx_vector;
jnx_vector *jnx_vector_init(void);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_vector_delete
 *  Description:  Deletes the vector data structure and its buckets, user assumes data responsibility FOR DELETING DATA
 * =====================================================================================
 */
void jnx_vector_delete(jnx_vector* vector);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_vector_insert
 *  Description:  Insert to the end of the vector 
 * =====================================================================================
 */
void jnx_vector_insert(jnx_vector *vector, void *value);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_vector_insert_at
 *  Description:  Inserts into a vector at position 
 * =====================================================================================
 */
void jnx_vector_insert_at(jnx_vector *vector, int position, void *value);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_vector_last
 *  Description:  removes the last element from the vector and returns its data
 * =====================================================================================
 */
void *jnx_vector_last(jnx_vector *vector);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_vector_remove_at
 *  Description:  Delete a vector positions data
 * =====================================================================================
 */
void jnx_vector_remove_at(jnx_vector *vector,int position);
#endif 
