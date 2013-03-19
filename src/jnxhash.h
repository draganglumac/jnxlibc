#ifndef __JNX_HASH_H__
#define __JNX_HASH_H__
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "jnxlist.h"
typedef struct bucketel {
    const char* origin_key;
    const char* origin_value;
} jnx_hash_bucket_el;
typedef struct hashel {
    jnx_list* bucket;
    int bucket_len;
    int used;
} jnx_hash_element;
typedef struct hashmap {
    jnx_hash_element* data;
    unsigned int size;
    unsigned int used_up;
} jnx_hashmap;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_hash_delete
 *  Description:  Deletes the hashelement lists and the hash elements, and the hashmap 
 * =====================================================================================
 */
void jnx_hash_delete(jnx_hashmap* hashmap);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_hash_init
 *  Description:  Returns a newly created hashmap and requires a init size i.e 1024
 * =====================================================================================
 */
jnx_hashmap* jnx_hash_init(unsigned int size);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_hash_delete
 *  Description:  Given a key and value this hashs and stores
 * =====================================================================================
 */
int jnx_hash_put(jnx_hashmap* hashmap, const char* key, void* value);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_hash_get
 *  Description:  Returns the value for a given key
 * =====================================================================================
 */
void* jnx_hash_get(jnx_hashmap* hashmap, const char* key);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_hash_get
 *  Description:  Allows access to the hash string function, returning and unsigned int
 * =====================================================================================
 */
int jnx_hash_string(const char* input, int map_size);
#endif
