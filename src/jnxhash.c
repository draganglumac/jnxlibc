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
			jnx_list_delete(temp);
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
			return (void*)bucketel->origin_value;
		}
		if (hashmap->data[index].bucket_len > 1) {
			jnx_node* head = hashmap->data[index].bucket->head;
			jnx_node* rewind_head = head;
			while (head) {
				jnx_hash_bucket_el* bucketel = head->_data;
				if (strcmp(bucketel->origin_key, key) == 0) {
					hashmap->data[index].bucket->head = rewind_head;
					return (void*)bucketel->origin_value;
				}
				head = head->next_node;
			}
			hashmap->data[index].bucket->head = rewind_head;
		}
	}
	return NULL;
}
int jnx_hash_get_keys(jnx_hashmap *hashmap,const char ***keys)
{
	int x, counter = 0;
	int offset = 1;
	int default_size = sizeof(char*);
	*keys = calloc(1,default_size);

	for(x = 0; x < hashmap->size; ++x)
	{
		if(hashmap->data[x].used)
		{
			jnx_node *head_of_buckets = hashmap->data[x].bucket->head;
			while(head_of_buckets){
				jnx_hash_bucket_el *current_bucket = head_of_buckets->_data;
				(*keys)[counter] = current_bucket->origin_key;
				++offset;
				*keys = realloc(*keys, (default_size * offset));
				head_of_buckets = head_of_buckets->next_node;
				++counter;
			}
		}
	}
	return counter;
}
int jnx_hash_put(jnx_hashmap* hashmap, const char* key, void* value)
{
	int index = jnx_hash_string(key, hashmap->size);
	if (hashmap->data[index].used == 0) {
		// we need to setup the bucket
		hashmap->data[index].bucket = jnx_list_init();
		hashmap->data[index].bucket_len = 0;
		//okay so bucket is ready to get given a KVP entry, so we'll use our bucket struct
		jnx_hash_bucket_el* current_bucket_el = malloc(sizeof(jnx_hash_bucket_el));
		current_bucket_el->origin_key = malloc(strlen(key) + 1);
		strncpy(current_bucket_el->origin_key,key,strlen(key) + 1);
		current_bucket_el->origin_value = value;//insert our bucket element...
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
		//If the list has been searched and the entry does not exist, we'll do a new insert
		jnx_hash_bucket_el* current_bucket_el = malloc(sizeof(jnx_hash_bucket_el));
		current_bucket_el->origin_key = malloc(strlen(key) + 1);
		strncpy(current_bucket_el->origin_key,key,strlen(key) + 1);
		current_bucket_el->origin_value = value;
		jnx_list_add(hashmap->data[index].bucket, current_bucket_el);
		hashmap->data[index].bucket_len++;
		hashmap->data[index].used = 1;
		hashmap->used_up++;
		//so the linked list length should be incremented....
	}

	return 0;
}
void* jnx_hash_delete_value(jnx_hashmap *hashmap,char *key)
{
	int index = jnx_hash_string(key, hashmap->size);
	jnx_node *head = hashmap->data[index].bucket->head;
	jnx_node *rewind = head;
	if(head == NULL)
	{
		return NULL;
	}	
	if(head->next_node == NULL)
	{
		jnx_hash_bucket_el *element = head->_data;
		void *data = (void*)element->origin_value;
		free(element->origin_key);
		free(element);
		jnx_list_delete(hashmap->data[index].bucket);
		hashmap->data[index].bucket = NULL;
		hashmap->data[index].used = 0;
		hashmap->data[index].bucket_len = 0;
		return data;
	}
	jnx_node *previous_node = NULL;
	while(head)
	{
		jnx_hash_bucket_el *bucketel = head->_data;
		if(strcmp(bucketel->origin_key,key) == 0)
		{
			if(previous_node == NULL)
			{
				//Start node in list
				void *data = (void*)bucketel->origin_value;
				free(bucketel->origin_key);
				free(bucketel);
				//lets set the hashmap to point to the next element of this list
				hashmap->data[index].bucket->head = head->next_node;
				///free this node in the list
				free(head);
				hashmap->data[index].bucket_len--;
				return data;
			}else
			{
				if(head->next_node == NULL)
				{
					///Last node in list
					///we have a previous node which will become the last node in the list now
					void *data = (void*)bucketel->origin_value;
					free(bucketel->origin_key);
					free(bucketel);
					free(head);	
					previous_node->next_node = NULL;
					head = rewind;
					hashmap->data[index].bucket_len--;
					return data;
				}else
				{
					//Middle node in list
					//set need to set the previous node, and the next node to link to each other
					void *data = (void*)bucketel->origin_value;
					free(bucketel->origin_key);
					free(bucketel);
					//set the previous node to jump to the next
					jnx_node *next_node = head->next_node;
					free(head);	
					previous_node->next_node = next_node;
					hashmap->data[index].bucket_len--;
					return data;
				}

			}

		}
		previous_node = head;
		head = head->next_node;
	}
	return NULL;
}
