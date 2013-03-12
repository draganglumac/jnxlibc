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

int find_index_of_child_for_key(jnx_B_tree *tree, jnx_B_tree_node *node, void *key)
{
    // Records are ordered so to make search efficient
    // (especially for arrays with lots of records)
    // we use binary chop, which is O(lg n) time.
   
    int last_index = node->count - 1; 
    int offset = node->count / 2;
    int next_index = offset;
    
    do
    {
        void *node_key = node->records[next_index]->key;
        int cf = tree->compare_function(node_key, key);

        if ( cf == 0 )
        {
            // Found exact match
            return next_index;
        }
        
        offset = offset / 2;
        if ( cf < 0 )
        {
            next_index = next_index + offset;
        }
        else
        {
            next_index = next_index - offset;
        }
    }
    while ( offset > 1 );

    return next_index;
}

void move_contents_from_index(jnx_B_tree_node *source, jnx_B_tree_node *target, int index)
{
    // Copy the RHS half of the records and children to new node
    memcpy((void *) target->records, 
           (const void*) (source->records + index),
           source->count - index - 1);
    memcpy((void *) target->children,
           (const void*) (source->children + index + 1),
           source->count - index);
    
    // Zero out the old records and children in the old node
    bzero((void *) (source->records + index), source->count - index - 1);
    bzero((void *) (source->children + index + 1), source->count - index);

    // Adjust record counts in the nodes
    source->count = source->count - index - 1;
    target->count = source->count;
}

void shift_right_from_index(jnx_B_tree_node *node, int index)
{
    // Shift records to the right of i by one position
    memcpy((void *) (node->records + index),
           (const void*) (node->records + index + 1),
           node->count - index + 1);
    // Shift children to the rigth of i + 1 by one position
    memcpy((void *) (node->children + index + 1),
           (const void*) (node->children + index + 2),
           node->count - index + 1);
}

int is_node_full(jnx_B_tree *tree, jnx_B_tree_node *node)
{
	if ( node->count == 2 * tree->order - 1 )
	{
		return 1;
	}

	return 0;
}

void split_child_at_index(jnx_B_tree *tree, jnx_B_tree_node *node, int child_index)
{
    int tree_order = tree->order;

    jnx_B_tree_node *temp = node->children[child_index];
    jnx_B_tree_node *sibling = new_node(tree_order, temp->is_leaf);
    
    move_contents_from_index(temp, sibling, tree_order);

    // Now rearrange "node" to fit the new record and its children
    record *middle = temp->records[tree_order - 1];
    int i = find_index_of_child_for_key(tree, node, middle->key);

    shift_right_from_index(node, i);
    
    // move the middle record up
    node->records[i] = middle;
    // attach newly created sibling
    node->children[i + i] = sibling;
    // finally, increment the record count in the current node
    node->count++;
}

void add_record_to_non_full_node(jnx_B_tree_node *node, record *r)
{
    // ToDo: Stub

    int size = node->count;
}

/* 
 * To guarantee that leaf node is never full, we split any full
 * node on the way down the tree, find the appropriate subtree
 * to traverse, and insert the node into the subtree.
 *
 * The only special case is if the root is a leaf node. We handle
 * this in jnx_B_tree_add API function.
 */
void insert_into_tree_at_node(jnx_B_tree *tree, jnx_B_tree_node *node, record *r)
{
   if ( node->is_leaf )
   {
       add_record_to_non_full_node(node, r);
       return;
   }
   
   int i = find_index_of_child_for_key(tree, node, r->key);
   
   if ( tree->compare_function(node->records[i]->key, r->key) == 0 )
   {
       // Same key so just replace the old record with the new one
       free(node->records[i]);
       node->records[i] = r;
       return;
   }

   if ( is_node_full(tree, node->children[i]) )
   {
       split_child_at_index(tree, node, i);
       
       if ( tree->compare_function(node->records[i]->key, r->key) < 0 )
       {
           // We're going to the right of the record that moved up
           i++;
       }
   }

   // Recurse down into the appropriate subtree
   insert_into_tree_at_node(tree, node->children[i], r);
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

    if ( is_node_full(tree, tree->root) )
    {
        jnx_B_tree_node *temp = tree->root;
        tree->root = new_node(tree->order, 0);
        tree->root->children[0] = temp;

        split_child_at_index(tree, tree->root, 0);

        tree->root->count = 1;
    }

    insert_into_tree_at_node(tree, tree->root, r);
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
