#include "jnxhash.h"
int jnx_hash_string(const char* input, int map_size)
{
    assert(input != 0 && input[0] != 0);
    int i;
    unsigned h = input[0];
    for (i = 1; input[i] != 0; ++i) {
        h = (h << 4) + input[i];
    }
    return h % map_size;
}
void jnx_hash_delete(jnx_hashmap* hashmap)
{
    jnx_hashmap* m = hashmap;
	int i;
	for(i = 0; i < hashmap->size; ++i)
	{
		jnx_list *temp = hashmap->data[i].bucket;
		if(temp != NULL)
		{
			free(temp);
		}
	}
	
	free(m->data);
    free(m);
}
jnx_hashmap* jnx_hash_init(unsigned int size)
{
    jnx_hashmap* hashmap = (jnx_hashmap*)malloc(sizeof(jnx_hashmap));
    hashmap->data = (jnx_hash_element*)calloc(size, sizeof(jnx_hash_element));
    hashmap->size = size;
    hashmap->used_up = 0;
    return hashmap;
}
void* jnx_hash_get(jnx_hashmap* hashmap, const char* key)
{
    int index = jnx_hash_string(key, hashmap->size);

    if (hashmap->data[index].used) {
        if (hashmap->data[index].bucket_len == 1) {
            jnx_node* head = hashmap->data[index].bucket->head;
            jnx_hash_bucket_el* bucketel = head->_data;
            return bucketel->origin_value;
        }
        if (hashmap->data[index].bucket_len > 1) {
            jnx_node* head = hashmap->data[index].bucket->head;
            jnx_node* rewind_head = head;
            while (head) {
                jnx_hash_bucket_el* bucketel = head->_data;
                if (strcmp(bucketel->origin_key, key) == 0) {
                    hashmap->data[index].bucket->head = rewind_head;
                    return bucketel->origin_value;
                }
                head = head->next_node;
            }
            hashmap->data[index].bucket->head = rewind_head;
        }
    }
    return NULL;
}
int jnx_hash_put(jnx_hashmap* hashmap, const char* key, void* value)
{
    int index = jnx_hash_string(key, hashmap->size);
    if (hashmap->data[index].used == NULL) {
        // we need to setup the bucket
        hashmap->data[index].bucket = jnx_list_make();
        //okay so bucket is ready to get given a KVP entry, so we'll use our bucket struct
        jnx_hash_bucket_el* current_bucket_el = malloc(sizeof(jnx_hash_bucket_el));
        current_bucket_el->origin_key = key;
        current_bucket_el->origin_value = value;
        //insert our bucket element...
        jnx_list_add(hashmap->data[index].bucket, current_bucket_el);
        hashmap->data[index].bucket_len++;
        hashmap->data[index].used++;
        hashmap->used_up++;
    } else {
        // So we are here assuming that the bucket exists so we're going to append to the existing bucket...
        //check to see if it already exists!
		jnx_node* head = hashmap->data[index].bucket->head;
            jnx_node* rewind_head = head;
            while (head) {
                jnx_hash_bucket_el* bucketel = head->_data;
                if (strcmp(bucketel->origin_key, key) == 0) {
					bucketel->origin_value = value;
                    hashmap->data[index].bucket->head = rewind_head;
                    return 0;
                }
                head = head->next_node;
            }
            hashmap->data[index].bucket->head = rewind_head;
		
        jnx_hash_bucket_el* current_bucket_el = malloc(sizeof(jnx_hash_bucket_el));
        current_bucket_el->origin_key = key;
        current_bucket_el->origin_value = value;
        jnx_list_add(hashmap->data[index].bucket, current_bucket_el);
        hashmap->data[index].bucket_len++;
        hashmap->data[index].used = 1;
        hashmap->used_up++;
        //so the linked list length should be incremented....
    }
    
    return 0;
}

