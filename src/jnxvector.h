/**
 * @file jnxvector.h
 * @brief The API for jnxlibc's implementation of vector
 *
 * #include <jnxc_headers/jnxvector.h>
 */
#ifndef __JNX_VECTOR_H__
#define __JNX_VECTOR_H__
/** 
 * @brief An internal implementation of record data
 */
typedef struct{
    int used;
    void *data;
}jnx_vector_record;
/** 
 * @brief The external structure and interface for jnx vector
 */
typedef struct{
    int count;
    jnx_vector_record **vector;
}jnx_vector;


/**
 * @fn jnx_vector_init(void)
 * @brief Function that initialises internal vector structures. Call this function
 *			before you call any other function from the API.
 */
jnx_vector *jnx_vector_init(void);

/**
 * @fn jnx_vector_delete(jnx_vector* vector)
 * @brief Deletes the vector and its data structures
 * @param vector points to the vector to delete
 *
 * @warning User assumes responsibility for deleting all data entries first
 */
void jnx_vector_delete(jnx_vector* vector);

/**
 * @fn jnx_vector_insert(jnx_vector *vector, void *value)
 * @brief inserts into the next position in the vector
 * @param vector points to the vector to insert into
 * @param value value for insertion cast into void*
 */
void jnx_vector_insert(jnx_vector *vector, void *value);

/**
 * @fn jnx_vector_insert_at(jnx_vector *vector, int position, void *value)
 * @brief inserts into the vector at a particular position, if it doesn't exist it will be crearted
 * @param vector 
 * @param position
 * @param value
 */
void jnx_vector_insert_at(jnx_vector *vector, int position, void *value);

/**
 * @fn jnx_vector_last(jnx_vector *vector)
 * @brief returns the last data entry in the vector and shortens it by 1
 * @param vector points to the vector to use
 *
 * @return returns void* data from the last vector entry
 */
void *jnx_vector_last(jnx_vector *vector);

/**
 * @fn jnx_vector_remove_at(jnx_vector *vector, int position)
 * @brief removes an entry at position in the vector
 * @param vector points to the target vector
 * @param position target position
 * @return returns the value, returns NULL on error 
 */
void* jnx_vector_remove_at(jnx_vector *vector,int position);
#endif 
