#ifndef __JNX_HASH_H__
#define __JNX_HASH_H__
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "jnxlist.h"
///Our bucket element is the stored unit inside each hash link list
typedef struct bucketel {
    const char* origin_key;
    const char* origin_value;
} jnx_hash_bucket_el;
typedef struct hashel {
    jnx_list* bucket;
    int bucket_len; //haha
    int used;
} jnx_hash_element;

typedef struct hashmap {
    jnx_hash_element* data;
    unsigned int size;
    unsigned int used_up;
} jnx_hashmap;

void jnx_hash_delete(jnx_hashmap* hashmap);
jnx_hashmap* jnx_hash_init(unsigned int size);
int jnx_hash_put(jnx_hashmap* hashmap, const char* key, void* value);
void* jnx_hash_get(jnx_hashmap* hashmap, const char* key);
int jnx_hash_string(const char* input, int map_size);
#endif
