#ifndef __JNX_HASH_H__
#define __JNX_HASH_H__
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "jnxlist.h"
/** @file jnxhash.h
 *  @brief API for jnxlibc's implementation of hashmap
 *
 *  #include <jnxc_headers/jnxhash.h>
 */
/**
 * @brief Internal container for a key value pair
 */
typedef struct bucketel {
    const char* origin_key;
    const char* origin_value;
} jnx_hash_bucket_el;
/**
 * @brief Internal container for key value pair lists
 */
typedef struct hashel {
    jnx_list* bucket;
    int bucket_len;
    int used;
} jnx_hash_element;
/* *
 * @brief External facing representaion of the hashmap
 */
typedef struct hashmap {
    jnx_hash_element* data;
    unsigned int size;
    unsigned int used_up;
} jnx_hashmap;

/** @fn jnx_hash_delete(jnx_hashmap *hashmap)
 *  @brief Deletes the hashelement lists and the hash elements, and the hashmap 
 *  @param hashmap* pointer to the map to be deleted
 */
void jnx_hash_delete(jnx_hashmap* hashmap);

/** @fn jnx_hash_init(unsigned int size)
 *  @brief Returns a newly created hashmap
 *  @param size initial size for the hashmap, unsigned int i.e 1024
 *  @return newly created hashmap with size
 */
jnx_hashmap* jnx_hash_init(unsigned int size);

/** @fn jnx_hash_put(jnx_hashmap* hashmap, const char *key, void *value)
 *  @brief Given a key and value this hashs and stores
 *  @param hashmap* pointer to the target hashmap
 *  @param key the key you want to associate with the value
 *  @param value data
 */
int jnx_hash_put(jnx_hashmap* hashmap, const char* key, void* value);

/** @fn jnx_hash_get(jnx_hashmap *hashmap, const char *key)
 *  @brief Returns the value for a given key
 *  @param hashmap pointer to hashmap for retrieval
 *  @param key for search and value retrieval
 *  @return void pointer to the data from value
 */
void* jnx_hash_get(jnx_hashmap* hashmap, const char* key);

/** @fn jnx_hash_string(const char *input, int map_size) 
 *  @brief  Allows access to the hash string function, returning and unsigned int
 *  @param input is the string to be hashed
 *  @param map_size is used to round the remainder of the string
 *  @return int of the hashed string
 */
int jnx_hash_string(const char* input, int map_size);
#endif
