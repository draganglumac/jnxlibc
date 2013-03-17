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

void test_insert_records_into_leaf_root()
{
    printf("- test_insert_records_into_leaf_root: ");

    jnx_B_tree *tree = jnx_B_tree_init(5, compare_pints);
    int data[] = { 42, 12, 56, 3, 27, 100, 31, 1, 47 };
    int i;

    for ( i = 0; i < 9; i++ )
    {
        jnx_B_tree_add(tree, data + i, data + i);
    }

    jnx_B_tree_node *root = tree->root;
    assert(root->count == 9);
    assert(compare_pints(root->records[0]->key, data + 7) == 0);
    assert(compare_pints(root->records[1]->key, data + 3) == 0);
    assert(compare_pints(root->records[2]->key, data + 1) == 0);
    assert(compare_pints(root->records[3]->key, data + 4) == 0);
    assert(compare_pints(root->records[4]->key, data + 6) == 0);
    assert(compare_pints(root->records[5]->key, data + 0) == 0);
    assert(compare_pints(root->records[6]->key, data + 8) == 0);
    assert(compare_pints(root->records[7]->key, data + 2) == 0);
    assert(compare_pints(root->records[8]->key, data + 5) == 0);

    jnx_B_tree_delete(tree);

    printf("OK\n"); 
}

void test_growing_to_depth_of_2()
{
    printf("- test_growing_to_depth_of_2: ");

    jnx_B_tree *tree = jnx_B_tree_init(3, compare_pints);

    int data[] = { 42, 12, 56, 3, 27, 100, 31, 1, 47 };

    int i;
    for ( i = 0; i < 9; i++ )
    {
        jnx_B_tree_add(tree, data + i, data + i);
    }

    jnx_B_tree_node *root = tree->root;
    assert(root->count == 1);
    assert(compare_pints(root->records[0]->key, data + 4) == 0);

    jnx_B_tree_node *first = root->children[0];
    assert(first->count == 3);
    assert(compare_pints(first->records[0]->key, data + 7) == 0);
    assert(compare_pints(first->records[1]->key, data + 3) == 0);
    assert(compare_pints(first->records[2]->key, data + 1) == 0);

    jnx_B_tree_node *second = root->children[1];
    assert(second->count == 5);
    assert(compare_pints(second->records[0]->key, data + 6) == 0);
    assert(compare_pints(second->records[1]->key, data + 0) == 0);
    assert(compare_pints(second->records[2]->key, data + 8) == 0);
    assert(compare_pints(second->records[3]->key, data + 2) == 0);
    assert(compare_pints(second->records[4]->key, data + 5) == 0);

    jnx_B_tree_delete(tree);

    printf("OK\n");
}

void test_spliting_a_leaf_node_that_is_not_root()
{
    printf("- test_spliting_a_leaf_node_that_is_not_root: ");

    jnx_B_tree *tree = jnx_B_tree_init(3, compare_pints);

    int data[] = { 42, 12, 56, 3, 27, 100, 31, 1, 47, 46 };

    int i;
    for ( i = 0; i < 10; i++ )
    {
        jnx_B_tree_add(tree, data + i, data + i);
    }

    jnx_B_tree_node *root = tree->root;
    assert(root->count == 2);
    assert(compare_pints(root->records[0]->key, data + 4) == 0);
    assert(compare_pints(root->records[1]->key, data + 8) == 0);

    jnx_B_tree_node *first = root->children[0];
    assert(first->count == 3);
    assert(compare_pints(first->records[0]->key, data + 7) == 0);
    assert(compare_pints(first->records[1]->key, data + 3) == 0);
    assert(compare_pints(first->records[2]->key, data + 1) == 0);

    jnx_B_tree_node *second = root->children[1];
    assert(second->count == 3);
    assert(compare_pints(second->records[0]->key, data + 6) == 0);
    assert(compare_pints(second->records[1]->key, data + 0) == 0);
    assert(compare_pints(second->records[2]->key, data + 9) == 0);

    jnx_B_tree_node *third = root->children[2];
    assert(third->count == 2);
    assert(compare_pints(third->records[0]->key, data + 2) == 0);
    assert(compare_pints(third->records[1]->key, data + 5) == 0);

    jnx_B_tree_delete(tree);

    printf("OK\n");
}

void test_growing__to_depth_of_3()
{
    printf("- test_growing__to_depth_of_3: ");

    jnx_B_tree *tree = jnx_B_tree_init(2, compare_pints);
    int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    int i;
    for( i = 0; i < 10; i++ ) 
    {
        jnx_B_tree_add(tree, data + i, data + i);
    }

    jnx_B_tree_node *root = tree->root;
    assert(root->count == 1);
    assert(root->is_leaf == 0);
    assert(root->records[1] == NULL && root->records[2] == NULL);
    assert(root->children[0] != NULL && root->children[1] != NULL);
    assert(root->children[2] == NULL && root->children[3] == NULL);
    assert(compare_pints(root->records[0]->key, data + 3) == 0);

    // Level 1 nodes
    jnx_B_tree_node *l1_0 = tree->root->children[0];
    assert(l1_0->count == 1);
    assert(l1_0->is_leaf == 0);
    assert(l1_0->records[1] == NULL && l1_0->records[2] == NULL);
    assert(l1_0->children[0] != NULL && l1_0->children[1] != NULL);
    assert(l1_0->children[2] == NULL && l1_0->children[3] == NULL);
    assert(compare_pints(l1_0->records[0]->key, data + 1) == 0);

    jnx_B_tree_node *l1_1 = tree->root->children[1];
    assert(l1_1->count == 2);
    assert(l1_1->is_leaf == 0);
    assert(l1_1->records[2] == NULL);
    assert(l1_1->children[0] != NULL && l1_1->children[1] != NULL && l1_1->children[2] != NULL);
    assert(l1_1->children[3] == NULL);
    assert(compare_pints(l1_1->records[0]->key, data + 5) == 0);
    assert(compare_pints(l1_1->records[1]->key, data + 7) == 0);
    
    // Level 2 leaf nodes
    jnx_B_tree_node *l2_0 = l1_0->children[0];
    assert(l2_0->count == 1);
    assert(l2_0->is_leaf == 1);
    assert(l2_0->records[1] == NULL && l2_0->records[2] == NULL);
    assert(l2_0->children[0] == NULL && l2_0->children[1] == NULL && l2_0->children[2] == NULL && l2_0->children[3] == NULL);
    assert(compare_pints(l2_0->records[0]->key, data + 0) == 0);
    
    jnx_B_tree_node *l2_1 = l1_0->children[1];
    assert(l2_1->count == 1);
    assert(l2_1->is_leaf == 1);
    assert(l2_1->records[1] == NULL && l2_1->records[2] == NULL);
    assert(l2_1->children[0] == NULL && l2_1->children[1] == NULL && l2_1->children[2] == NULL && l2_1->children[3] == NULL);
    assert(compare_pints(l2_1->records[0]->key, data + 2) == 0);
    
    jnx_B_tree_node *l2_2 = l1_1->children[0];
    assert(l2_2->count == 1);
    assert(l2_2->is_leaf == 1);
    assert(l2_2->records[1] == NULL && l2_2->records[2] == NULL);
    assert(l2_2->children[0] == NULL && l2_2->children[1] == NULL && l2_2->children[2] == NULL && l2_2->children[3] == NULL);
    assert(compare_pints(l2_2->records[0]->key, data + 4) == 0);
    
    jnx_B_tree_node *l2_3 = l1_1->children[1];
    assert(l2_3->count == 1);
    assert(l2_3->is_leaf == 1);
    assert(l2_3->records[1] == NULL && l2_3->records[2] == NULL);
    assert(l2_3->children[0] == NULL && l2_3->children[1] == NULL && l2_3->children[2] == NULL && l2_3->children[3] == NULL);
    assert(compare_pints(l2_3->records[0]->key, data + 6) == 0);
   
    jnx_B_tree_node *l2_4 = l1_1->children[2];
    assert(l2_4->count == 2);
    assert(l2_4->is_leaf == 1);
    assert(l2_4->records[2] == NULL);
    assert(l2_4->children[0] == NULL && l2_4->children[1] == NULL && l2_4->children[2] == NULL && l2_4->children[3] == NULL);
    assert(compare_pints(l2_4->records[0]->key, data + 8) == 0);
    assert(compare_pints(l2_4->records[1]->key, data + 9) == 0);
    
    jnx_B_tree_delete(tree);

    printf("OK\n");
}

void test_splitting_inner_node()
{
    printf("- test_splitting_inner_node: ");

    jnx_B_tree *tree = jnx_B_tree_init(2, compare_pints);
    int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

    int i;
    for( i = 0; i < 14; i++ ) 
    {
        jnx_B_tree_add(tree, data + i, data + i);
    }

    jnx_B_tree_node *root = tree->root;
    assert(root->count == 2);
    assert(root->is_leaf == 0);
    assert(root->records[2] == NULL);
    assert(root->children[0] != NULL && root->children[1] != NULL && root->children[2] != NULL);
    assert(root->children[3] == NULL);
    assert(compare_pints(root->records[0]->key, data + 3) == 0);
    assert(compare_pints(root->records[1]->key, data + 7) == 0);

    // Level 1 nodes
    jnx_B_tree_node *l1_0 = tree->root->children[0];
    assert(l1_0->count == 1);
    assert(l1_0->is_leaf == 0);
    assert(l1_0->records[1] == NULL && l1_0->records[2] == NULL);
    assert(l1_0->children[0] != NULL && l1_0->children[1] != NULL);
    assert(l1_0->children[2] == NULL && l1_0->children[3] == NULL);
    assert(compare_pints(l1_0->records[0]->key, data + 1) == 0);

    jnx_B_tree_node *l1_1 = tree->root->children[1];
    assert(l1_1->count == 1);
    assert(l1_1->is_leaf == 0);
    assert(l1_1->records[1] == NULL && l1_1->records[2] == NULL);
    assert(l1_1->children[0] != NULL && l1_1->children[1] != NULL);
    assert(l1_1->children[2] == NULL && l1_1->children[3] == NULL);
    assert(compare_pints(l1_1->records[0]->key, data + 5) == 0);

    jnx_B_tree_node *l1_2 = tree->root->children[2];
    assert(l1_2->count == 2);
    assert(l1_2->is_leaf == 0);
    assert(l1_2->records[2] == NULL);
    assert(l1_2->children[0] != NULL && l1_2->children[1] != NULL && l1_2->children[2] != NULL);
    assert(l1_2->children[3] == NULL);
    assert(compare_pints(l1_2->records[0]->key, data + 9) == 0);
    assert(compare_pints(l1_2->records[1]->key, data + 11) == 0);
    
    // Level 2 leaf nodes
    jnx_B_tree_node *l2_0 = l1_0->children[0];
    assert(l2_0->count == 1);
    assert(l2_0->is_leaf == 1);
    assert(l2_0->records[1] == NULL && l2_0->records[2] == NULL);
    assert(l2_0->children[0] == NULL && l2_0->children[1] == NULL && l2_0->children[2] == NULL && l2_0->children[3] == NULL);
    assert(compare_pints(l2_0->records[0]->key, data + 0) == 0);
    
    jnx_B_tree_node *l2_1 = l1_0->children[1];
    assert(l2_1->count == 1);
    assert(l2_1->is_leaf == 1);
    assert(l2_1->records[1] == NULL && l2_1->records[2] == NULL);
    assert(l2_1->children[0] == NULL && l2_1->children[1] == NULL && l2_1->children[2] == NULL && l2_1->children[3] == NULL);
    assert(compare_pints(l2_1->records[0]->key, data + 2) == 0);
    
    jnx_B_tree_node *l2_2 = l1_1->children[0];
    assert(l2_2->count == 1);
    assert(l2_2->is_leaf == 1);
    assert(l2_2->records[1] == NULL && l2_2->records[2] == NULL);
    assert(l2_2->children[0] == NULL && l2_2->children[1] == NULL && l2_2->children[2] == NULL && l2_2->children[3] == NULL);
    assert(compare_pints(l2_2->records[0]->key, data + 4) == 0);
    
    jnx_B_tree_node *l2_3 = l1_1->children[1];
    assert(l2_3->count == 1);
    assert(l2_3->is_leaf == 1);
    assert(l2_3->records[1] == NULL && l2_3->records[2] == NULL);
    assert(l2_3->children[0] == NULL && l2_3->children[1] == NULL && l2_3->children[2] == NULL && l2_3->children[3] == NULL);
    assert(compare_pints(l2_3->records[0]->key, data + 6) == 0);
   
    jnx_B_tree_node *l2_4 = l1_2->children[0];
    assert(l2_4->count == 1);
    assert(l2_4->is_leaf == 1);
    assert(l2_4->records[1] == NULL && l2_4->records[2] == NULL);
    assert(l2_4->children[0] == NULL && l2_4->children[1] == NULL && l2_4->children[2] == NULL && l2_4->children[3] == NULL);
    assert(compare_pints(l2_4->records[0]->key, data + 8) == 0);

    jnx_B_tree_node *l2_5 = l1_2->children[1];
    assert(l2_5->count == 1);
    assert(l2_5->is_leaf == 1);
    assert(l2_5->records[1] == NULL && l2_5->records[2] == NULL);
    assert(l2_5->children[0] == NULL && l2_5->children[1] == NULL && l2_5->children[2] == NULL && l2_5->children[3] == NULL);
    assert(compare_pints(l2_5->records[0]->key, data + 10) == 0);

    jnx_B_tree_node *l2_6 = l1_2->children[2];
    assert(l2_6->count == 2);
    assert(l2_6->is_leaf == 1);
    assert(l2_6->records[2] == NULL);
    assert(l2_6->children[0] == NULL && l2_6->children[1] == NULL && l2_6->children[2] == NULL && l2_6->children[3] == NULL);
    assert(compare_pints(l2_6->records[0]->key, data + 12) == 0);
    assert(compare_pints(l2_6->records[1]->key, data + 13) == 0);
    
    jnx_B_tree_delete(tree);
    
    printf("OK\n");
}

void test_lookup_in_empty_tree()
{
    printf("- test_lookup_in_empty_tree: ");

    jnx_B_tree *tree = jnx_B_tree_init(2, compare_pints);
    int key = 42;

    void *val = jnx_B_tree_lookup(tree, (void *) &key);
    assert(val == NULL);

    jnx_B_tree_delete(tree);
    
    printf("OK\n");
}

void test_lookup_in_single_record_tree()
{
    printf("- test_lookup_in_single_record_tree: ");

    jnx_B_tree *tree = jnx_B_tree_init(2, compare_pints);
    int key = 42, key2 = -3;

    jnx_B_tree_add(tree, &key, &key);

    void *val = jnx_B_tree_lookup(tree, (void *) &key);
    assert(*((int *) val) == 42);

    val = jnx_B_tree_lookup(tree, &key2); 
    assert(val == NULL);

    jnx_B_tree_delete(tree);
    
    printf("OK\n");
}

void test_lookup_in_leaf_root()
{
    printf("- test_insert_records_into_leaf_root: ");

    jnx_B_tree *tree = jnx_B_tree_init(5, compare_pints);
    int data[] = { 42, 12, 56, 3, 27, 100, 31, 1, 47 };
    int i;

    for ( i = 0; i < 9; i++ )
    {
        jnx_B_tree_add(tree, data + i, data + i);
    }

    void *val = jnx_B_tree_lookup(tree, (void *) (data + 2));
    assert(*((int *) val) == 56);

    val = jnx_B_tree_lookup(tree, (void *) (data + 7));
    assert(*((int *) val) == 1);

    val = jnx_B_tree_lookup(tree, (void *) (data + 5));
    assert(*((int *) val) == 100);

    int bob = -1;
    val = jnx_B_tree_lookup(tree, (void *) &bob);
    assert(val == NULL);
   
    jnx_B_tree_delete(tree);

    printf("OK\n"); 
}

void test_lookup_in_tree_of_depth_3()
{
    printf("- test_lookup_in_tree_of_depth_3: ");

    jnx_B_tree *tree = jnx_B_tree_init(2, compare_pints);
    int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

    int i;
    for( i = 0; i < 14; i++ ) 
    {
        jnx_B_tree_add(tree, data + i, data + i);
    }

    int *val = (int *) (jnx_B_tree_lookup(tree, data));
    assert(*val == 1);

    val = (int *) (jnx_B_tree_lookup(tree, data + 7));
    assert(*val == 8);

    val = (int *) (jnx_B_tree_lookup(tree, data + 13));
    assert(*val == 14);

    int bob = -1;
    val = (int *) (jnx_B_tree_lookup(tree, (void *) &bob));
    assert(val == NULL);

    jnx_B_tree_delete(tree);

    printf("OK\n");
}

void test_lookup_does_not_modify_tree()
{
    printf("- test_lookup_does_not_modify_tree: ");

    jnx_B_tree *tree = jnx_B_tree_init(2, compare_pints);
    int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

    int i;
    for( i = 0; i < 14; i++ ) 
    {
        jnx_B_tree_add(tree, data + i, data + i);
    }

    int *val = (int *) (jnx_B_tree_lookup(tree, data));
    val = (int *) (jnx_B_tree_lookup(tree, data + 7));
    val = (int *) (jnx_B_tree_lookup(tree, data + 13));
    int bob = -1;
    val = (int *) (jnx_B_tree_lookup(tree, (void *) &bob));

    jnx_B_tree_node *root = tree->root;
    assert(root->count == 2);
    assert(root->is_leaf == 0);
    assert(root->records[2] == NULL);
    assert(root->children[0] != NULL && root->children[1] != NULL && root->children[2] != NULL);
    assert(root->children[3] == NULL);
    assert(compare_pints(root->records[0]->key, data + 3) == 0);
    assert(compare_pints(root->records[1]->key, data + 7) == 0);

    // Level 1 nodes
    jnx_B_tree_node *l1_0 = tree->root->children[0];
    assert(l1_0->count == 1);
    assert(l1_0->is_leaf == 0);
    assert(l1_0->records[1] == NULL && l1_0->records[2] == NULL);
    assert(l1_0->children[0] != NULL && l1_0->children[1] != NULL);
    assert(l1_0->children[2] == NULL && l1_0->children[3] == NULL);
    assert(compare_pints(l1_0->records[0]->key, data + 1) == 0);

    jnx_B_tree_node *l1_1 = tree->root->children[1];
    assert(l1_1->count == 1);
    assert(l1_1->is_leaf == 0);
    assert(l1_1->records[1] == NULL && l1_1->records[2] == NULL);
    assert(l1_1->children[0] != NULL && l1_1->children[1] != NULL);
    assert(l1_1->children[2] == NULL && l1_1->children[3] == NULL);
    assert(compare_pints(l1_1->records[0]->key, data + 5) == 0);

    jnx_B_tree_node *l1_2 = tree->root->children[2];
    assert(l1_2->count == 2);
    assert(l1_2->is_leaf == 0);
    assert(l1_2->records[2] == NULL);
    assert(l1_2->children[0] != NULL && l1_2->children[1] != NULL && l1_2->children[2] != NULL);
    assert(l1_2->children[3] == NULL);
    assert(compare_pints(l1_2->records[0]->key, data + 9) == 0);
    assert(compare_pints(l1_2->records[1]->key, data + 11) == 0);
    
    // Level 2 leaf nodes
    jnx_B_tree_node *l2_0 = l1_0->children[0];
    assert(l2_0->count == 1);
    assert(l2_0->is_leaf == 1);
    assert(l2_0->records[1] == NULL && l2_0->records[2] == NULL);
    assert(l2_0->children[0] == NULL && l2_0->children[1] == NULL && l2_0->children[2] == NULL && l2_0->children[3] == NULL);
    assert(compare_pints(l2_0->records[0]->key, data + 0) == 0);
    
    jnx_B_tree_node *l2_1 = l1_0->children[1];
    assert(l2_1->count == 1);
    assert(l2_1->is_leaf == 1);
    assert(l2_1->records[1] == NULL && l2_1->records[2] == NULL);
    assert(l2_1->children[0] == NULL && l2_1->children[1] == NULL && l2_1->children[2] == NULL && l2_1->children[3] == NULL);
    assert(compare_pints(l2_1->records[0]->key, data + 2) == 0);
    
    jnx_B_tree_node *l2_2 = l1_1->children[0];
    assert(l2_2->count == 1);
    assert(l2_2->is_leaf == 1);
    assert(l2_2->records[1] == NULL && l2_2->records[2] == NULL);
    assert(l2_2->children[0] == NULL && l2_2->children[1] == NULL && l2_2->children[2] == NULL && l2_2->children[3] == NULL);
    assert(compare_pints(l2_2->records[0]->key, data + 4) == 0);
    
    jnx_B_tree_node *l2_3 = l1_1->children[1];
    assert(l2_3->count == 1);
    assert(l2_3->is_leaf == 1);
    assert(l2_3->records[1] == NULL && l2_3->records[2] == NULL);
    assert(l2_3->children[0] == NULL && l2_3->children[1] == NULL && l2_3->children[2] == NULL && l2_3->children[3] == NULL);
    assert(compare_pints(l2_3->records[0]->key, data + 6) == 0);
   
    jnx_B_tree_node *l2_4 = l1_2->children[0];
    assert(l2_4->count == 1);
    assert(l2_4->is_leaf == 1);
    assert(l2_4->records[1] == NULL && l2_4->records[2] == NULL);
    assert(l2_4->children[0] == NULL && l2_4->children[1] == NULL && l2_4->children[2] == NULL && l2_4->children[3] == NULL);
    assert(compare_pints(l2_4->records[0]->key, data + 8) == 0);

    jnx_B_tree_node *l2_5 = l1_2->children[1];
    assert(l2_5->count == 1);
    assert(l2_5->is_leaf == 1);
    assert(l2_5->records[1] == NULL && l2_5->records[2] == NULL);
    assert(l2_5->children[0] == NULL && l2_5->children[1] == NULL && l2_5->children[2] == NULL && l2_5->children[3] == NULL);
    assert(compare_pints(l2_5->records[0]->key, data + 10) == 0);

    jnx_B_tree_node *l2_6 = l1_2->children[2];
    assert(l2_6->count == 2);
    assert(l2_6->is_leaf == 1);
    assert(l2_6->records[2] == NULL);
    assert(l2_6->children[0] == NULL && l2_6->children[1] == NULL && l2_6->children[2] == NULL && l2_6->children[3] == NULL);
    assert(compare_pints(l2_6->records[0]->key, data + 12) == 0);
    assert(compare_pints(l2_6->records[1]->key, data + 13) == 0);
    
    jnx_B_tree_delete(tree);

    printf("OK\n"); 
}

int main()
{

    printf("Running B-tree tests...\n");

    // Insertion tests
    test_new_node();
    test_new_empty_tree();
    test_insert_first_record_into_tree();
    test_insert_records_into_leaf_root();
    test_growing_to_depth_of_2();
    test_spliting_a_leaf_node_that_is_not_root();
    test_growing__to_depth_of_3();
    test_splitting_inner_node();
    
    // Lookup tests
    test_lookup_in_empty_tree();
    test_lookup_in_single_record_tree();
    test_lookup_in_leaf_root();
    test_lookup_in_tree_of_depth_3();
    test_lookup_does_not_modify_tree();

    printf("B-tree tests completed.\n");

    return 0;
}
