/*
 * =====================================================================================
 *
 *       Filename:  jnxbtree.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/05/2013 06:28:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __JNX_B_TREE__
#define __JNX_B_TREE__

typedef struct {
    void *key;
    void *value;
} record;

/*
 * Callback function that the user needs to supply and which compares two keys,
 * and effectivelly two records as keys and values live together in a node record.
 *
 * The B-tree keys are required to follow some order, so with respect to that order
 * the function should return:
 *     - an integer < 0 if first parameter comes before second parameter
 *     - a 0 if first and second parameters are equal
 *     - an integer > 0 if the first parameter comes after the second parameter
 * 
 * Typically, these values can be -1, 0, and 1 respectivelly.
 */
typedef int (*compare_keys) (void*, void*);

typedef struct B_tree_node {
    int count;
    int is_leaf;
    record **records;
    struct B_tree_node **children;
} jnx_B_tree_node;

typedef struct {
    int order;
    compare_keys compare_function;
    jnx_B_tree_node *root;
} jnx_B_tree;

/*  
 * Initialise B-tree with order, which is the minimum number of records in a non-root node,
 * and callback function which compares the keys of two records.
 *
 * The function returns a pointer to the new empty B-tree.
 */
jnx_B_tree* jnx_B_tree_init(int order, compare_keys callback);

/*
 * Add a record (key, value) to the B-tree.
 */
void jnx_B_tree_add(jnx_B_tree *tree, void *key, void *value);

/* 
 * Retreive the value for key from the B-tree.
 *
 * The function returns the pointer to the value, or NULL if the key is not in the B-tree.
 */
void *jnx_B_tree_lookup(jnx_B_tree *tree, void *key);

/*
 * Remove a record with key equal to parameter key from the B-tree.
 *
 * If the record with key "key" is not in the tree, the tree is unchanged.
 */
void jnx_B_tree_remove(jnx_B_tree *tree, void *key);

/*
 * Delete the B-tree.
 */
void jnx_B_tree_delete(jnx_B_tree* tree);

#endif // __JNX_B_TREE__
