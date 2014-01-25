/*
 * =====================================================================================
 *
 *       Filename:  jnxmem.c
 *
 *    Description: Memory manager that uses internal datastructures for tracking 
 *
 *        Version:  1.0
 *        Created:  01/03/14 18:16:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxmem.h"
//--------------------internal list implementation------------------------//
typedef struct node {
	void* _data;
	struct node* prev_node;
	struct node* next_node;
} __internal_list_node;
typedef struct list {
	struct node* head;
	struct node* tail;
	int counter;
} __internal_list;

__internal_list* __internal_list_create(void)
{
	__internal_list *list = malloc(sizeof(__internal_list));
	list->head = NULL;
	list->tail = NULL;
	list->counter = 0;
	return list;
}
void __internal_list_add(__internal_list* A, void* _datain)
{
	if(A->head == NULL)
	{
		__internal_list_node *node = malloc(sizeof(__internal_list_node));
		node->_data = _datain; 
		node->next_node = NULL;
		node->prev_node = NULL;
		A->head = node;
		A->tail = node;
		A->counter++;
		return;
	}
	__internal_list_node *marker = A->head;
	while(A->head)
	{
		__internal_list_node *current_node = A->head;
		if(!A->head->next_node)
		{	
			__internal_list_node *node = malloc(sizeof(__internal_list_node));
			node->_data = _datain;
			node->next_node = NULL;
			node->prev_node = current_node;	
			current_node->next_node = node;
			A->counter++;
			A->tail = node;
			A->head = marker;
			return;
		}
		A->head = A->head->next_node;
	}
	A->head = marker;
}
void* __internal_list_remove(__internal_list** A)
{
	if((*A)->head == NULL)
	{
		return NULL;
	}
	if((*A)->head->next_node == NULL)
	{
		void *data = (*A)->head->_data;
		free((*A)->head);
		(*A)->head = NULL;
		(*A)->counter--;
		return data;
	}
	__internal_list_node *origin_head = (*A)->head;
	while((*A)->head)
	{
		__internal_list_node *next = (*A)->head->next_node;
		if(next->next_node == NULL)
		{
			void *data = next->_data;
			free(next);
			(*A)->head->next_node = NULL;
			(*A)->head = origin_head; 
			(*A)->counter--;
			return data;
		}
		(*A)->head = next;
	}
	(*A)->head = origin_head;
	return NULL;
}
void __internal_list_destroy(__internal_list** A)
{
	if((*A) == NULL)
	{
		printf("__internal_list_destroy: No list\n");
		return;
	}
	if((*A)->head == NULL)
	{
		(*A) = NULL;
		return;
	}
	__internal_list_node *current = (*A)->head;
	if(!current->next_node){ 

		free(current);
		free(*A);
		(*A) = NULL;
		return;
	}
	while(current)
	{
		__internal_list_node *current_node = current;
		current = current->next_node; 
		free(current_node);
	}
	free(*A);
	(*A) = NULL;
}
//--------------------internal list implementation------------------------//


//--------------------internal btree implementation------------------------//
typedef struct B_tree_record {
	void *key; /**< The key for the record. */
	void *value; /**< The value for the record.  */
} record;

typedef int (*__internal_compare_keys) (void*, void*);

typedef struct B_tree_node {
	int count; /**< How many records are in the node. */
	int is_leaf; /**< Is it a leaf node (set to 1) or an internal node (set to 0). */
	record **records; /**< The records contained in the node. */
	struct B_tree_node **children; /**< The subtrees of the node. A leaf node has each of its subtrees set to NULL. */
} __internal_btree_node;

typedef struct B_tree {
	int order; /**< The order of the tree, i.e. the minimum number of records a non-root node can have. */
	__internal_compare_keys compare_function; /**< Callback function to compare keys. @see compare_keys */
	__internal_btree_node *root; /**< The root node of the B-tree. */
} __internal_btree;

__internal_btree_node *__internal_new_node(int order, int is_leaf)
{
	__internal_btree_node *node = calloc(1, sizeof(__internal_btree_node));

	node->count = 0; // just to be explicit, calloc will already zero the field
	node->is_leaf = is_leaf;
	node->records = calloc(2 * order, sizeof(record*));
	node->children = calloc(2 * order + 1, sizeof(__internal_btree_node*)); 

	return node;
}

void __internal_delete_node(__internal_btree_node *node)
{
	free(node->records);
	free(node->children);
	free(node);
}

void __internal_move_contents_from_index(__internal_btree_node *source, __internal_btree_node *target, int index)
{
	// Copy the RHS half of the records and children to new node
	memmove((void *) target->records, 
			(const void*) (source->records + index),
			(source->count / 2) * sizeof(record *));
	memmove((void *) target->children,
			(const void*) (source->children + index),
			(source->count / 2 + 1) * sizeof(__internal_btree_node *));

	// Zero out the old records and children in the old node
	int new_count = source->count / 2;
	bzero((void *) (source->records + index - 1), (new_count + 1) * sizeof(record *));
	bzero((void *) (source->children + index), (new_count + 1) * sizeof(__internal_btree_node *));

	// Adjust record counts in the nodes
	source->count = new_count;
	target->count = new_count;
}

void __internal_shift_records_right_from_index(__internal_btree_node *node, int index)
{
	memmove((void *)(node->records + (index + 1)),
			(const void*)(node->records + index),
			(node->count - index) * sizeof(record *));
}

void __internal_shift_children_right_from_index(__internal_btree_node *node, int index)
{
	memmove((void *)(node->children + (index + 1)),
			(const void*)(node->children + index),
			(node->count + 1 - index) * sizeof(__internal_btree_node *));
}

void __internal_shift_records_left_from_index(__internal_btree_node *node, int index)
{
	memmove((void *)(node->records + index - 1),
			(const void*)(node->records + index),
			(node->count - index) * sizeof(record *));
	bzero((void *) (node->records + node->count - 1), sizeof(record *));
}

void __internal_shift_children_left_from_index(__internal_btree_node *node, int index)
{
	memmove((void *)(node->children + index - 1),
			(const void*)(node->children + index),
			(node->count - index + 1) * sizeof(__internal_btree_node *));
	bzero((void *) (node->children + node->count), sizeof(__internal_btree_node *));
}

int __internal_is_node_full(__internal_btree *tree, __internal_btree_node *node)
{
	if ( node->count == 2 * tree->order - 1 )
	{
		return 1;
	}

	return 0;
}

int __internal_is_node_empty(__internal_btree_node *node)
{
	if ( node == NULL || node->count == 0 )
	{
		return 1;
	}

	return 0;
}

int __internal_find_index_for_record(__internal_btree *tree, __internal_btree_node *node, record *r)
{
	// Records are ordered so to make search efficient
	// (especially for arrays with lots of records)
	// we use binary chop, which is O(lg n) time.

	if ( node->count == 0 )
	{
		return 0;
	}

	int left_bound = 0;
	int right_bound = node->count - 1;
	int curr_index = (right_bound - left_bound) / 2; 

	do
	{
		void *key = node->records[curr_index]->key;
		int cf = tree->compare_function(key, r->key);

		if ( cf == 0 )
		{
			// Found exact match
			return curr_index;
		}

		if ( cf < 0 )
		{
			left_bound = curr_index + 1;
		}
		else
		{
			right_bound = curr_index - 1;
		}

		curr_index = left_bound + (right_bound - left_bound) / 2;
	}
	while ( right_bound >= left_bound );

	return curr_index;
}

void __internal_split_child_at_index(__internal_btree *tree, __internal_btree_node *node, int child_index)
{
	int tree_order = tree->order;

	__internal_btree_node *temp = node->children[child_index];
	__internal_btree_node *sibling = __internal_new_node(tree_order, temp->is_leaf);
	record *middle = temp->records[tree_order - 1];

	__internal_move_contents_from_index(temp, sibling, tree_order);

	// Now rearrange "node" to fit the new record and its children
	int i = __internal_find_index_for_record(tree, node, middle);
	if ( node->records[i] != NULL )
	{
		// Shift only if the record slot is not empty
		__internal_shift_records_right_from_index(node, i);
		__internal_shift_children_right_from_index(node, i + 1);
	}

	// move the middle record up
	node->records[i] = middle;
	// attach newly created sibling
	node->children[i + 1] = sibling;
	// finally, increment the record count in the current node
	node->count++;
}

void __internal_add_record_to_non_full_leaf(__internal_btree *tree, __internal_btree_node *node, record *r)
{
	int i = __internal_find_index_for_record(tree, node, r);

	if ( node->records[i] == NULL )
	{
		node->records[i] = r;
		node->count++;
	}
	else
	{
		void *key = node->records[i]->key;

		if ( tree->compare_function(key, r->key) == 0 )
		{
			// Replace the value only scenario
			free(node->records[i]);
			node->records[i] = r;
		}
		else
		{
			__internal_shift_records_right_from_index(node, i);
			node->records[i] = r;
			node->count++;
		}
	}  
}

/* 
 * To guarantee that leaf node is never full, we split any full
 * node on the way down the tree, find the appropriate subtree
 * to traverse, and insert the node into the subtree.
 *
 * The only special case is if the root is a leaf node. We handle
 * this in __internal_btree_add API function.
 */
void __internal_insert_into_tree_at_node(__internal_btree *tree, __internal_btree_node *node, record *r)
{
	if ( node->is_leaf )
	{
		__internal_add_record_to_non_full_leaf(tree, node, r);
		return;
	}

	int i = __internal_find_index_for_record(tree, node, r);

	if ( i < node->count )
	{
		if ( tree->compare_function(node->records[i]->key, r->key) == 0 )
		{
			// Same key so just replace the old record with the new one
			free(node->records[i]);
			node->records[i] = r;
			return;
		}
	}

	if ( __internal_is_node_full(tree, node->children[i]) )
	{
		__internal_split_child_at_index(tree, node, i);

		if ( tree->compare_function(node->records[i]->key, r->key) == 0 )
		{
			// Case when the node that moved up is actually the node we want to insert
			free(node->records[i]);
			node->records[i] = r;
			return;
		}

		if ( tree->compare_function(node->records[i]->key, r->key) < 0 )
		{
			// We're going to the right of the record that moved up
			i++;
		}
	}

	// Recurse down into the appropriate subtree
	__internal_insert_into_tree_at_node(tree, node->children[i], r);
}

void __internal_delete_node_and_subtrees(__internal_btree_node *node)
{
	if ( node == NULL )
	{
		return;
	}

	// Delete all the children first
	int i;
	for ( i = 0; i <= node->count; i++ )
	{
		if ( node->children[i] != NULL )
		{
			__internal_delete_node_and_subtrees(node->children[i]);
		}
	}

	// Free memory for all the records
	for (i = 0; i < node->count; i++ )
	{
		if ( node->records[i] != NULL )
		{
			free(node->records[i]);
		}
	}

	// Finally, free the node itself
	__internal_delete_node(node);
}

void *__internal_find_value_for_key_in_node(__internal_btree *tree, __internal_btree_node *node, void *key)
{
	if ( __internal_is_node_empty(node) )
	{ 
		return NULL;
	}

	record r;
	r.key = key;

	int i =  __internal_find_index_for_record(tree, node, &r);

	if ( node->records[i] != NULL )
	{
		if ( tree->compare_function(r.key, node->records[i]->key) == 0 )
		{
			return node->records[i]->value;
		}
	}

	return __internal_find_value_for_key_in_node(tree, node->children[i], key);
}

record *__internal_find_rightmost_record_in_subtree_at_node(__internal_btree_node *node)
{
	__internal_btree_node *temp = node;

	while ( temp->children[temp->count] != NULL )
	{
		temp = temp->children[temp->count];
	}

	return temp->records[temp->count - 1];
}

record *__internal_find_leftmost_record_in_subtree_at_node(__internal_btree_node *node)
{
	__internal_btree_node *temp = node;

	while ( temp->children[0] != NULL )
	{
		temp = temp->children[0];
	}

	return temp->records[0];
}

void __internal_merge_subtrees_around_index(__internal_btree *tree, __internal_btree_node *node, int index)
{
	__internal_btree_node *root = tree->root;

	__internal_btree_node *first = node->children[index];
	__internal_btree_node *second = node->children[index + 1];

	first->records[first->count] = node->records[index];
	first->count++;

	memmove((void *)(first->records + first->count),
			(const void *)(second->records),
			second->count * sizeof(record *));

	memmove((void *)(first->children + first->count),
			(const void *)(second->children),
			(second->count + 1) * sizeof(__internal_btree_node *));

	// Adjust the count
	first->count += second->count;

	// Fill the gap in node by shifting records and children
	// that are to the right of index by 1 position to the left.
	node->children[index + 1] = first;
	__internal_shift_records_left_from_index(node, index + 1);
	__internal_shift_children_left_from_index(node, index + 2);

	// Removed the record from node so adjust the count
	node->count--;

	// Delete the second node, i.e. free its memory on the heap
	__internal_delete_node(second);

	// Since root is the only node that can have less than t-1 records
	// we have to account for the fact when root has only one record
	// and that record's just been pushed down to the first child as a
	// result of the merge.
	if ( node == root )
	{
		tree->root = first;
		__internal_delete_node(root);
	}
}

record *__internal_delete_record_from_node(__internal_btree *tree, __internal_btree_node *node, record *r)
{
	int i = __internal_find_index_for_record(tree, node, r);

	if ( node->is_leaf )
	{
		if ( node == tree->root || node->count >= tree->order)
		{
			if ( i < node->count )
			{
				if ( tree->compare_function(node->records[i]->key, r->key) == 0 )
				{
					record *temp = node->records[i];
					__internal_shift_records_left_from_index(node, i + 1);
					node->count--;

					return temp;
				}
			}
		}

		return NULL;
	}

	// Index 'i' returned by __internal_find_index_for_record is one of two things:
	// - index of the record itself, if the key is in this node
	// - index of the appropriate sutbree, if key is not in this node
	// This unfortunately means that we have to guard against going past the
	// records array upper bound which is at most node->count + 1
	int rec_i = i < node->count ? i : node->count - 1; 
	record *node_rec = node->records[rec_i];
	record *temp = NULL, *retval = NULL;

	if ( tree->compare_function(node_rec->key, r->key) == 0 )
	{
		// It is safe not to check for NULL children here because
		// we are in an internal node, and since by definition
		// a B-tree grows from the leaves up, and the only way to
		// add a level is to split a node at the previous, a record
		// in an inner node will have both left and right subtrees.

		if ( node->children[rec_i]->count >= tree->order )
		{
			temp = __internal_find_rightmost_record_in_subtree_at_node(node->children[rec_i]);

			node->records[rec_i] = malloc(sizeof(record));
			node->records[rec_i]->key = temp->key;
			node->records[rec_i]->value = temp->value;

			retval = __internal_delete_record_from_node(tree, node->children[rec_i], temp);

			free(node_rec);
		}
		else if ( node->children[rec_i + 1]->count >= tree->order )
		{
			temp = __internal_find_leftmost_record_in_subtree_at_node(node->children[rec_i + 1]);

			node->records[rec_i] = malloc(sizeof(record));
			node->records[rec_i]->key = temp->key;
			node->records[rec_i]->value = temp->value;

			retval = __internal_delete_record_from_node(tree, node->children[rec_i + 1], temp);

			free(node_rec);
		}
		else
		{
			__internal_merge_subtrees_around_index(tree, node, rec_i);
			retval = __internal_delete_record_from_node(tree, node->children[rec_i], r);
		}

		return retval;
	}

	if ( node->children[i]->count < tree->order )
	{
		// guard the boundaries
		int b = i > 0 ? i - 1 : 0;
		int a = i < node->count ? i + 1 : node->count;

		__internal_btree_node *subtree = node->children[i];

		if ( a != i && node->children[a]->count >= tree->order )
		{
			__internal_btree_node *sibling = node->children[a];

			// Shift records around
			subtree->records[subtree->count] = node->records[i];
			node->records[i] = sibling->records[0];
			subtree->children[subtree->count + 1] = sibling->children[0];

			// Fix up sibling 
			__internal_shift_records_left_from_index(sibling, 1);
			__internal_shift_children_left_from_index(sibling, 1);

			// Adjust counts
			subtree->count++;
			sibling->count--;
		}
		else if ( b != i && node->children[b]->count >= tree->order )
		{
			__internal_btree_node *sibling = node->children[b];
			int rec_i = i < node->count ? i : node->count - 1;

			// Shift records around
			__internal_shift_records_right_from_index(subtree, 0);
			__internal_shift_children_right_from_index(subtree, 0);
			subtree->records[0] = node->records[rec_i];
			node->records[rec_i] = sibling->records[sibling->count - 1];
			subtree->children[0] = sibling->children[sibling->count];

			// Fix up the sibling
			sibling->records[sibling->count - 1] = NULL;
			sibling->children[sibling->count] = NULL;

			// Adjust counts
			subtree->count++;
			sibling->count--;
		}
		else
		{
			__internal_merge_subtrees_around_index(tree, node, i);
		}
	}

	return __internal_delete_record_from_node(tree, node->children[i], r);
}


/*
 * ===========================================
 *  API functions implementations start here!
 * ===========================================
 */

__internal_btree* __internal_btree_create(int order, __internal_compare_keys callback)
{
	if ( order <= 1 )
	{
		return NULL;
	}

	__internal_btree *tree = calloc(1, sizeof(__internal_btree));

	tree->order = order;
	tree->compare_function = callback;
	tree->root = __internal_new_node(tree->order, 1);

	return tree; 
}

void __internal_btree_add(__internal_btree *tree, void *key, void *value)
{
	if ( tree == NULL)
	{
		return;
	}

	record *r = malloc(sizeof(record));
	r->key = key;
	r->value = value;

	if ( __internal_is_node_full(tree, tree->root) )
	{
		__internal_btree_node *temp = tree->root;
		tree->root = __internal_new_node(tree->order, 0);
		tree->root->children[0] = temp;

		__internal_split_child_at_index(tree, tree->root, 0);
	}

	__internal_insert_into_tree_at_node(tree, tree->root, r);
}

void *__internal_btree_lookup(__internal_btree *tree, void *key)
{
	if ( tree == NULL )
	{
		return NULL;
	}

	return __internal_find_value_for_key_in_node(tree, tree->root, key);
}

void __internal_btree_remove(__internal_btree *tree, void *key_in, void** key_out, void **val_out )
{
	record *temp = NULL;

	if ( tree == NULL || tree->root->count == 0 )
	{
		return;
	}

	record *r = malloc(sizeof(record));
	r->key = key_in;
	r->value = key_in;

	temp = __internal_delete_record_from_node(tree, tree->root, r);
	if (temp != NULL)
	{
		if (key_out != NULL)
			*key_out = temp->key;
		if (val_out != NULL)
			*val_out = temp->value;
	}

	free(temp);	
	free(r);
}

void __internal_btree_destroy(__internal_btree* tree)
{
	if ( tree == NULL )
	{
		return;
	}

	__internal_delete_node_and_subtrees(tree->root);

	free(tree);
}

static void append_keys_from_node(__internal_btree_node *node, __internal_list *keys)
{
	int i;
	for (i = 0; i < node->count; i++ )
		__internal_list_add(keys, node->records[i]->key);
}

static void collect_keys_from_node(__internal_btree_node *node, __internal_list *keys)
{
	if (node == NULL)
		return;

	if ( node->is_leaf )
	{
		append_keys_from_node(node, keys);
		return;
	}

	int i;
	for (i = 0; i <= node->count; i++ )
	{
		collect_keys_from_node(node->children[i], keys);
		if ( i < node->count )
			__internal_list_add(keys, node->records[i]->key);
	}

}

void __internal_btree_keys(__internal_btree *tree, __internal_list *keys)
{
	collect_keys_from_node(tree->root, keys);
}

//--------------------internal btree implementation------------------------//
typedef enum { FREE, ALLOC }jnx_mem_memtrace_state;
typedef struct{
	void *ptr;
	size_t size;
	char *file;
	char *function;
	int line;
	jnx_mem_memtrace_state state;
	
}jnx_mem_item;

static __internal_btree *memtree = NULL;

size_t jnx_mem_get_total_number_allocations()
{
	size_t c = 0;
	__internal_list *l = __internal_list_create();
	__internal_btree_keys(memtree,l);
	c = l->counter;
	__internal_list_destroy(&l);
	return c;
}
int __internal_compare_callback(void *A, void *B)
{
	if(A > B)
	{
		return -1;
	}
	if(B > A)
	{
		return 1;
	}
	return 0;
}
static inline void jnx_mem_new(void *ptr, size_t size,char* file,const char *function,int line)
{
	jnx_mem_item *m = malloc(sizeof(jnx_mem_item));
	if(m == NULL)
	{
		printf("Error with allocation\n [%zu(kb)]",(size /1024));
		return;
	}
	m->ptr = ptr;
	m->size = size;
	m->state = ALLOC;
	m->file = strdup(file);
	m->function = strdup(function);
	m->line = line;

	if(memtree == NULL)
	{
		__internal_compare_keys c = __internal_compare_callback;
		memtree = __internal_btree_create(sizeof(int),c);
	}
	__internal_btree_add(memtree,ptr,m);
}
void* jnx_mem_malloc(size_t size,char *file,const char *function,int line)
{
	void *p = malloc(size);	
	jnx_mem_new(p,size,file,function,line);
	return p;
}
void* jnx_mem_calloc(size_t num,size_t size,char *file,const char *function,int line)
{
	void *p = calloc(num,size);
	jnx_mem_new(p,size,file,function,line);
	return p;
}
void* jnx_mem_realloc(void *ptr,size_t size,char *file,const char *function,int line)
{
	void *p = realloc(ptr,size);
	jnx_mem_new(p,size,file,function,line);
	return p;
}
void jnx_mem_free(void *ptr)
{
	jnx_mem_item *m = __internal_btree_lookup(memtree,ptr);
	if(m)
	{
		m->state = FREE;
	}
	free(ptr);
}
