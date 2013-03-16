/*
 * =====================================================================================
 *
 *       Filename:  test_jnxbplustree.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/05/2013 06:29:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "../src/jnxbtree.h"

extern jnx_B_tree_node* new_node(int, int);
extern int find_index_of_child_for_key(jnx_B_tree *tree, jnx_B_tree_node *node, void *key);

void test_new_node()
{
    // Since this is not an API funciton and is private
    // there's no need to test for order <= 1, since the
    // API functions take care of this case.
    printf ("- test_new_node: ");
    
    jnx_B_tree_node *n = new_node(2, 0);
    assert(n != NULL);
    assert(n->count == 0);
    assert(n->is_leaf == 0);
    free(n);

    n = new_node(5, 1);
    assert(n != NULL);
    assert(n->count == 0);
    assert(n->is_leaf == 1);
    free(n);
    
    printf("OK\n");
}

int compare_pints(void *first, void *second)
{
    int *n1 = (int *) first;
    int *n2 = (int *) second;

    if ( *n1 == *n2 )
    {
        return 0;
    }
    else if ( *n1 < *n2 )
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void test_find_index_of_child_for_key()
{

}

void test_new_empty_tree()
{
    printf("- test_new_tree: ");
    
    jnx_B_tree *tree = jnx_B_tree_init(-1, compare_pints);
    assert(tree == NULL);

    tree = jnx_B_tree_init(0, compare_pints);
    assert(tree == NULL);

    tree = jnx_B_tree_init(1, compare_pints);
    assert(tree == NULL);

    tree = jnx_B_tree_init(2, compare_pints);
    assert(tree != NULL);
    assert(tree->order == 2);
    assert(tree->compare_function == compare_pints);
    assert(tree->root != NULL);
    assert(tree->root->count == 0);
    jnx_B_tree_delete(tree);

    tree = jnx_B_tree_init(5, compare_pints);
    assert(tree != NULL);
    assert(tree->order == 5);
    assert(tree->compare_function == compare_pints);
    assert(tree->root != NULL);
    assert(tree->root->count == 0);
    jnx_B_tree_delete(tree);

    printf("OK\n");
}

void test_insert_first_record_into_tree()
{
    printf("- test_insert_first_record_into_tree: ");

    jnx_B_tree *tree = jnx_B_tree_init(2, compare_pints);
    int kv = 42;
    jnx_B_tree_add(tree, (void *) &kv, (void *) &kv);
    jnx_B_tree_node *root = tree->root;

    assert(tree != NULL);
    assert(root != NULL);
    assert(root->records[0] != NULL);

    int *key = (int *) (root->records[0]->key);
    int *val = (int *) (root->records[0]->value);
    assert(key == &kv && *key == kv);
    assert(val == &kv && *val == kv);
    assert(root->count == 1);

    // test replacing the value in i a single-record tree
    int v2 = 24;
    jnx_B_tree_add(tree, (void *) &kv, (void *) &v2);
    key = (int *) (root->records[0]->key);
    val = (int *) (root->records[0]->value);
    assert(key == &kv && *key == kv);
    assert(val == &v2 && *val == v2);
    assert(root->count == 1);

    jnx_B_tree_delete(tree);
    
    printf("OK\n");
}

int main()
{

    printf("Running B-tree tests...\n");

    test_new_node();
    test_new_empty_tree();
    test_insert_first_record_into_tree();

    printf("B-tree tests completed.\n");

    return 0;
}
