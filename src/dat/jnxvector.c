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
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include "jnxvector.h"
#include "jnxmem.h"
jnx_vector *jnx_vector_create(void)
{
    jnx_vector *vector = JNX_MEM_MALLOC(sizeof(jnx_vector));
    vector->count = 0;
    vector->vector = NULL;
    return vector;
}
jnx_vector_record *jnx_vector_record_create(void *value)
{
    jnx_vector_record *record = JNX_MEM_MALLOC(sizeof(record));
    record->data = value;
    record->used = 1;

    return record;
}
jnx_vector_record *jnx_vector_record_create_empty()
{
    jnx_vector_record *record = JNX_MEM_MALLOC(sizeof(record));
    record->data = NULL;
    record->used = 0;
    return record;
}
//One of the primary reasons not to JNX_MEM_FREE data here is we don't know where it belongs too
//and it may cause a segfault if its on the stack
void jnx_vector_destroy(jnx_vector* vector)
{
    int x = 0;
    for ( x = 0; x < vector->count; ++x )
    {
        JNX_MEM_FREE(vector->vector[x]);
    }
    JNX_MEM_FREE(vector->vector);
    JNX_MEM_FREE(vector);
}
void jnx_vector_grow(jnx_vector **vector, int increment)
{
    int resize = (*vector)->count + increment;
    jnx_vector_record **temp = realloc((*vector)->vector,resize * sizeof(jnx_vector_record));
    if(temp == NULL) 
    {
        printf("Error with reallocing\n");
        exit(0);
    }
    else{
        (*vector)->vector = temp;
    }
}
void jnx_vector_fillspace(jnx_vector **vector,int start, int end)
{
    while(start < end)
    {
        (*vector)->vector[start] = jnx_vector_record_create_empty();
        ++start;
    }
}
void jnx_vector_insert_at(jnx_vector *vector, int position, void *value)
{
    if(position > vector->count)
    {
        int different = position - vector->count;
        jnx_vector_grow(&vector,different);
        jnx_vector_fillspace(&vector,vector->count,vector->count + different + 1);
        vector->count = vector->count + different;
        vector->vector[vector->count]->data = value;
        vector->vector[vector->count]->used = 1;
        vector->count++;
    }
    else
    {
        vector->vector[position]->used = 1;
        vector->vector[position]->data = value;
    }
}
void* jnx_vector_remove_at(jnx_vector *vector,int position)
{
    if(vector->vector[position]->used)
    {
        void *data = vector->vector[position]->data;
	   	vector->vector[position]->data = NULL;
        vector->vector[position]->used = 0;
		return data;
   	}
	return NULL;
}
void jnx_vector_insert(jnx_vector *vector, void *value)
{
    jnx_vector_record *record = jnx_vector_record_create(value);
    jnx_vector_grow(&vector,1); 
    vector->vector[vector->count] = record;
    vector->count++;
}
void *jnx_vector_last(jnx_vector *vector)
{
    if(vector->count == 0) 
    {
        printf("Cannot pop from an empty vector\n");
        return NULL;
    }
    void *data = vector->vector[vector->count -1 ]->data;
    JNX_MEM_FREE(vector->vector[vector->count -1]);
    vector->count--;
    vector->vector = realloc(vector->vector, (vector->count) * sizeof(void*));
    return data;
}
