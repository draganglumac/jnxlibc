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
 *         Author:  Dragan Glumac (dragan.glumac@gmail.com), 
 *   Organization:  
 *
 * =====================================================================================
 */

/** @file jnxbtree.h
 * @brief The API for jnxlibc's implementation of B-trees.
 *
 * #include <jnxc_headers/jnxbtree.h>
 */

#ifndef __JNX_B_TREE__
#define __JNX_B_TREE__


/**
 * @brief Internal representation of a (key, value) pair.
 */
typedef struct B_tree_record {
    void *key; /**< The key for the record. */
    void *value; /**< The value for the record.  */
} record;


/**
 * @brief Type for the callback function to determine the order of two keys.
 *
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


/**
 * @brief Internal representation of a node in a B-tree.
 */
typedef struct B_tree_node {
    int count; /**< How many records are in the node. */
    int is_leaf; /**< Is it a leaf node (set to 1) or an internal node (set to 0). */
    record **records; /**< The records contained in the node. */
    struct B_tree_node **children; /**< The subtrees of the node. A leaf node has each of its subtrees set to NULL. */
} jnx_B_tree_node;


/**
 * @brief Internal representation of the whole B-tree.
 */
typedef struct B_tree {
    int order; /**< The order of the tree, i.e. the minimum number of records a non-root node can have. */
    compare_keys compare_function; /**< Callback function to compare keys. @see compare_keys */
    jnx_B_tree_node *root; /**< The root node of the B-tree. */
} jnx_B_tree;


/** @fn jnx_B_tree* jnx_B_tree_init(int order, compare_keys callback)
 * @brief Create and initialise a new B-tree. This function must be called first, and once
 * per tree, before any other tree operations are called.
 *
 * Initialise B-tree with order and callback function which compares the keys of two records.
 *
 * @param order The order of the tree, i.e. the minumum number of records held in a 
 *                  non-root node.
 * @param callback The function that takes to keys as void pointers, and returns an
 *                     integer which represents whether the first key is "before", "equal"
 *                     or "after" the second key according to some key ordering. The function
 *                     returns an integer less than 0 if first key is before second key,
 *                     zero if both keys are equal, and an integer greater than 0 if the
 *                     first key is after the second key.
 *
 * @return The function returns a pointer to the new empty B-tree.
 */
jnx_B_tree* jnx_B_tree_init(int order, compare_keys callback);


/** @fn void jnx_B_tree_add(jnx_B_tree *tree, void *key, void *value)
 * @brief Add a record (key, value) to the B-tree.
 *
 * This function takes a key and a value for the record, creates the appropriate record
 * and inserts it into the tree. If a record with the same key already exists in the tree
 * the function will just replace the value for that record.
 *
 * @param tree The B-tree to which new record is added
 * @param key The key for the record
 * @param value The value for the record
 *
 * @warning The key and value are not copied they are just used as pointer assignments,
 * so it's important to think about the lifecycle of keys and values you pass to the function.
 */
void jnx_B_tree_add(jnx_B_tree *tree, void *key, void *value);


/** @fn void jnx_B_tree_lookup(jnx_B_tree *tree, void *key)
 * @brief Retreive the value for key from the B-tree.
 *
 * The function returns the pointer to the value, or NULL if the key is not in the B-tree.
 *
 * @param tree The B-tree in which to look up the value for key.
 * @param key The key for lookup.
 * 
 * @return Pointer to the value, or NULL if key is not in the tree.
 *
 * @warning The value is just a pointer to, not a copy of the actual value so it is very important
 * to think carefully before you free its memory as it'll be freed from the B-tree record also.
 */
void *jnx_B_tree_lookup(jnx_B_tree *tree, void *key);


/** @fn void jnx_B_tree_remove(jnx_B_tree *tree, void *key)
 * @brief Remove from the B-tree the record which matches the passed in key.
 *
 * If the record with @a key is in the @a tree, the record is removed and
 * the tree is rebalanced if necessary. If the record with @a key
 * is not in the @a tree, the tree is unchanged.
 *
 * @param tree The B-tree from which to remove the record.
 * @param key The key for which to remove the record.
 *
 * @warning This operation will clean up the internal structures but will not attempt to free
 * the memory for the key and value of the deleted record. It's worth bearing this in mind to
 * avoid memory leaks.
 */
void jnx_B_tree_remove(jnx_B_tree *tree, void *key);


/** @fn void jnx_B_tree_delete(jnx_B_tree *tree)
 * @brief Delete the B-tree.
 *
 * Delete all the nodes and records of the B-tree. Upon function return the tree will be empty.
 *
 * @param tree The B-tree to delete.
 */
void jnx_B_tree_delete(jnx_B_tree* tree);

#endif // __JNX_B_TREE__
