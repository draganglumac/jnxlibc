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

void insert_into_nonfull_node(jnx_B_tree_node *node, record *r)
{
    // Stub
}

/*
 ********************************************
 * API functions implementations start here!
 ********************************************
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

    int max_records = 2 * tree->order - 1;
    jnx_B_tree_node *root = tree->root;
    
    record *r = malloc(sizeof(record));
    r->key = key;
    r->value = value;

    if ( root->count == max_records )
    {
        jnx_B_tree_node *temp = root;
        tree->root = new_node(tree->order, 0);
        tree->root->children[0] = temp;
    }
    else
    {
        insert_into_nonfull_node(tree->root, r);
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
