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
jnx_vector *jnx_vector_init(void)
{
    jnx_vector *vector = malloc(sizeof(jnx_vector));
    vector->count = 0;
    vector->vector = malloc(sizeof(void*));
    return vector;
}
void jnx_vector_delete(jnx_vector* vector)
{ 
    if(vector == NULL) { return; }
    free(vector->vector);
    free(vector);
}
void jnx_vector_push(jnx_vector *vector, void *value)
{
    if(vector->count == 0)
    {
        vector->vector[vector->count] = value;
        vector->count++;
    }else{
        jnx_vector *temp = realloc(vector->vector,(vector->count + 1) * sizeof(void*));
        vector->vector[vector->count] = value;
        vector->count++;
    }
}
void *jnx_vector_pop(jnx_vector *vector)
{
    if(vector->count == 0) 
    {
        printf("Cannot pop from an empty vector\n");
        return NULL;
    }
    void *data = vector->vector[vector->count -1 ];
    vector->count--;
    vector->vector = realloc(vector->vector, (vector->count) * sizeof(void*));
    return data;
}
