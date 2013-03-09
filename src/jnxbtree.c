/*
 * =====================================================================================
 *
 *       Filename:  jnxbtree.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/05/2013 06:28:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>

#include "jnxbtree.h"


jnx_B_tree_node *new_node(int order, int is_leaf)
{
    jnx_B_tree_node *node = calloc(1, sizeof(jnx_B_tree_node));
    
    node->count = 0; // just to be explicit, calloc will already zero the field
    node->is_leaf = is_leaf;
    node->records = calloc(2 * order, sizeof(record*));
    node->children = calloc(2 * order + 1, sizeof(jnx_B_tree_node*)); 

    return node;
}

int find_index_for_record(jnx_B_tree *tree, jnx_B_tree_node *node, record *r)
{
    // Stub
    return 0;
}

int shift_right(jnx_B_tree *tree, jnx_B_tree * node, int start_index)
{
	// Stub
	return 0;
}

int is_node_full(jnx_B_tree *tree, jnx_B_tree_node *node)
{
	if ( node->count == 2 * tree->order - 1 )
	{
		return 1;
	}

	return 0;
}

jnx_B_tree_node *find_subtree_for_record(jnx_B_tree *tree, jnx_B_tree_node *node, record *r)
{
	int i = find_index_for_record(tree, node, r);
	return node->children[i];
}

void split_child_at_index(jnx_B_tree *tree, jnx_B_tree_node *node, int child_index)
{
    int tree_order = tree->order;

    jnx_B_tree_node *temp = node->children[child_index];
    jnx_B_tree_node *sibling = new_node(tree_order, temp->is_leaf);
    
    // Copy the RHS half of the records and children to new node
    memcpy((void *) sibling->records, 
           (const void*) (temp->records + tree_order),
           tree_order - 1);
    memcpy((void *) sibling->children,
           (const void*) (temp->children + tree_order + 1),
           tree_order);
    // Zero out the old records and children in the old node
    bzero((void *) (temp->records + tree_order), tree_order);
    bzero((void *) (temp->children + tree_order + 1), tree_order);

    // Adjust record counts in the nodes
    temp->count = tree_order - 1;
    sibling->count = tree_order - 1;

    // Now rearrange "node" to fit the new record and its children
    record *middle = temp->records[tree_order - 1];
    int i = find_index_for_record(tree, node, middle);
    // shift records to the right of i by one position
    memcpy((void *) (node->records + i),
           (const void*) (node->records + i + 1),
           node->count - i + 1);
    // shift children to the rigth of i + 1 by one position
    memcpy((void *) (node->children + i + 1),
           (const void*) (node->children + i + 2),
           node->count - i + 1);
    // move the middle record up
    node->records[i] = middle;
    // attach newly created sibling
    node->children[i + i] = sibling;
}

void insert_into_tree_node(jnx_B_tree *tree, jnx_B_tree_node *node, jnx_B_tree_node *parent, record *r)
{
	if ( ! node->is_leaf )
	{
		int i = find_child_index_for_record(tree, node, r);
    	insert_into_child_at_index(tree, node, i, r);
	}
    
	if ( is_node_full(tree, node) )
    {
        jnx_B_tree_node *temp = node;
        node = new_node(tree->order, 0);
        node->children[0] = temp;

        split_child_at_index(tree, tree->root, 0);
    }
	else
	{
		
	}
}

/*
 * ===========================================
 *  API functions implementations start here!
 * ===========================================
 */

jnx_B_tree* jnx_B_tree_init(int order, compare_keys callback)
{
    jnx_B_tree *tree = calloc(1, sizeof(jnx_B_tree));
    
    tree->order = order;
    tree->compare_function = callback;
    tree->root = NULL;

    return tree; 
}

void jnx_B_tree_add(jnx_B_tree *tree, void *key, void *value)
{
    record *r = malloc(sizeof(record));
    r->key = key;
    r->value = value;

	if ( tree->root->is_leaf )
	{
		if ( is_node_full(tree->root) )
		{
			jnx_B_tree_node *temp = tree->root;
			tree->root = new_node(tree->order, 0);
			tree->root->children[0] = temp;

			split_child_at_index(tree, tree->root, 0);

			if ( r->key < tree->root->records[0]->key )
			{
				add_record_to_non_full_node(tree->root->children[0], r);
			}
			else if ( r->key > tree->root->records[0]->key )
			{
				add_record_to_non_full_node(tree->root->children[1], r);
			}
			else
			{
				// Update the value - same key
				tree->root->records[0]->value = r->value;
			}
		}
		else
		{
			add_record_to_non_full_node(tree->root, r);
		}
	}
	else
	{
		int i = find_index_of_child_for_key(tree, tree->root, r->key);
		insert_record_into_child_at_index(tree, tree->root, i, r);
	}
}

void *jnx_B_tree_lookup(jnx_B_tree *tree, void *key)
{
    // Stub
    return NULL;
}

void jnx_B_tree_remove(jnx_B_tree *tree, void *key)
{
    // Stub
}

void jnx_B_tree_delete(jnx_B_tree* tree)
{
    // Stub
}
