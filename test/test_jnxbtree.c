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

int main()
{

    printf("Running B-tree tests...\n");

    test_new_node();

    printf("B-tree tests completed.\n");

    return 0;
}
