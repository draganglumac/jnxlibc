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
    vector->vector = NULL;
    return vector;
}
void jnx_vector_delete(jnx_vector* vector)
{
    int x = 0;
    while(x < vector->count)
    {
        if(vector->vector[x] != NULL)
        {
            free(vector->vector[x]);
        }
        ++x;
    }    
    free(vector->vector);
    free(vector);

}
void jnx_vector_grow(jnx_vector **vector)
{
    int resize = (*vector)->count;
    resize++;
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
void jnx_vector_insert(jnx_vector *vector, int position, void *value)
{

}
void jnx_vector_push(jnx_vector *vector, void *value)
{
    jnx_vector_record *record = malloc(sizeof(jnx_vector_record));
    record->data = value;
    jnx_vector_grow(&vector); 
    vector->vector[vector->count] = record;
    vector->count++;
}
void *jnx_vector_pop(jnx_vector *vector)
{
    if(vector->count == 0) 
    {
        printf("Cannot pop from an empty vector\n");
        return NULL;
    }
    void *data = vector->vector[vector->count -1 ]->data;
    free(vector->vector[vector->count -1]);
    vector->count--;
    vector->vector = realloc(vector->vector, (vector->count) * sizeof(void*));
    return data;
}
