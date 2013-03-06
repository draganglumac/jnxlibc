/*
 * =====================================================================================
 *
 *       Filename:  jnxbplustree.h
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
 * Callback function that the user needs to supply and which compares keys of two records.
 *
 * The B-tree keys are required to follow some order, so with respect to that order
 * the function should return:
 *     - an integer < 0 if first parameter comes before second parameter
 *     - a 0 if first and second parameters are equal
 *     - an integer > 0 if the first parameter comes after the second parameter
 * 
 * Typically, these values can be -1, 0, and 1 respectivelly.
 */
typedef int (*compare) (record*, record*);

typedef struct {
    int count;
    record **records;
    void **children;
} jnx_B_tree_node;

typedef struct {
    jnx_B_tree_node *root;
} jnx_B_tree;

/*  
 * Initialise B-tree with order, which is the minimum number of records in a non-root node,
 * and callback function which compares the keys of two records.
 */
jnx_B_tree* jnx_B_tree_init(int order, compare callback);

#endif // __JNX_B_TREE__
