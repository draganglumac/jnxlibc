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
 *         Author: Dragan Glumac
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "jnxlog.h"
#include "jnxbtree.h"
#include "jnxterm.h"
#include "jnxlist.h"

extern jnx_btree_node* new_node(int, int);
extern void delete_node(jnx_btree_node*);

void test_new_node() {
	// Since this is not an API funciton and is private
	// there's no need to test for order <= 1, since the
	// API functions take care of this case.
	printf ("- test_new_node: ");

	jnx_btree_node *n = new_node(2, 0);
	assert(n != NULL);
	assert(n->count == 0);
	assert(n->is_leaf == 0);
	delete_node(n);

	n = new_node(5, 1);
	assert(n != NULL);
	assert(n->count == 0);
	assert(n->is_leaf == 1);
	delete_node(n);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

int compare_pints(void *first, void *second) {
	int *n1 = (int *) first;
	int *n2 = (int *) second;

	return *n1 - *n2;
}

int compare_pchars(void *first, void *second) {
	char *f = (char *) first;
	char *s = (char *) second;

	return (*f - *s);
}

char *int_node_contents(jnx_btree_node *node) {
	char *contents = calloc(128, 1);

	int i;
	for ( i = 0; i < node->count; i++ ) {
		char next[16];
		sprintf(next, "%d ", *((int *) node->records[i]->key));
		strcat(contents, next);
	}

	return contents;
}

char *char_node_contents(jnx_btree_node *node) {
	char *contents = calloc(128, 1);

	int i;
	for ( i = 0; i < node->count; i++ ) {
		char next[2];
		sprintf(next, "%c", *((char *) node->records[i]->key));
		strcat(contents, next);
	}

	return contents;
}

void test_new_empty_tree() {
	JNX_LOGC(JLOG_DEBUG,"- test_new_tree: ");

	jnx_btree *tree = jnx_btree_create(-1, compare_pints);
	assert(tree == NULL);

	tree = jnx_btree_create(0, compare_pints);
	assert(tree == NULL);

	tree = jnx_btree_create(1, compare_pints);
	assert(tree == NULL);

	tree = jnx_btree_create(2, compare_pints);
	assert(tree != NULL);
	assert(tree->order == 2);
	assert(tree->compare_function == compare_pints);
	assert(tree->root != NULL);
	assert(tree->root->count == 0);
	jnx_btree_destroy(tree);

	tree = jnx_btree_create(5, compare_pints);
	assert(tree != NULL);
	assert(tree->order == 5);
	assert(tree->compare_function == compare_pints);
	assert(tree->root != NULL);
	assert(tree->root->count == 0);
	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_insert_first_record_into_tree() {
	JNX_LOGC(JLOG_DEBUG,"- test_insert_first_record_into_tree: ");

	jnx_btree *tree = jnx_btree_create(2, compare_pints);
	int kv = 42;
	jnx_btree_add(tree, (void *) &kv, (void *) &kv);
	jnx_btree_node *root = tree->root;

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
	jnx_btree_add(tree, (void *) &kv, (void *) &v2);
	key = (int *) (root->records[0]->key);
	val = (int *) (root->records[0]->value);
	assert(key == &kv && *key == kv);
	assert(val == &v2 && *val == v2);
	assert(root->count == 1);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_insert_records_into_leaf_root() {
	JNX_LOGC(JLOG_DEBUG,"- test_insert_records_into_leaf_root: ");

	jnx_btree *tree = jnx_btree_create(5, compare_pints);
	int data[] = { 42, 12, 56, 3, 27, 100, 31, 1, 47 };
	int i;

	for ( i = 0; i < 9; i++ ) {
		jnx_btree_add(tree, data + i, data + i);
	}

	jnx_btree_node *root = tree->root;
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

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_growing_to_depth_of_2() {
	JNX_LOGC(JLOG_DEBUG,"- test_growing_to_depth_of_2: ");

	jnx_btree *tree = jnx_btree_create(3, compare_pints);

	int data[] = { 42, 12, 56, 3, 27, 100, 31, 1, 47 };

	int i;
	for ( i = 0; i < 9; i++ ) {
		jnx_btree_add(tree, data + i, data + i);
	}

	jnx_btree_node *root = tree->root;
	assert(root->count == 1);
	assert(compare_pints(root->records[0]->key, data + 4) == 0);

	jnx_btree_node *first = root->children[0];
	assert(first->count == 3);
	assert(compare_pints(first->records[0]->key, data + 7) == 0);
	assert(compare_pints(first->records[1]->key, data + 3) == 0);
	assert(compare_pints(first->records[2]->key, data + 1) == 0);

	jnx_btree_node *second = root->children[1];
	assert(second->count == 5);
	assert(compare_pints(second->records[0]->key, data + 6) == 0);
	assert(compare_pints(second->records[1]->key, data + 0) == 0);
	assert(compare_pints(second->records[2]->key, data + 8) == 0);
	assert(compare_pints(second->records[3]->key, data + 2) == 0);
	assert(compare_pints(second->records[4]->key, data + 5) == 0);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_spliting_a_leaf_node_that_is_not_root() {
	JNX_LOGC(JLOG_DEBUG,"- test_spliting_a_leaf_node_that_is_not_root: ");

	jnx_btree *tree = jnx_btree_create(3, compare_pints);

	int data[] = { 42, 12, 56, 3, 27, 100, 31, 1, 47, 46 };

	int i;
	for ( i = 0; i < 10; i++ ) {
		jnx_btree_add(tree, data + i, data + i);
	}

	jnx_btree_node *root = tree->root;
	assert(root->count == 2);
	assert(compare_pints(root->records[0]->key, data + 4) == 0);
	assert(compare_pints(root->records[1]->key, data + 8) == 0);

	jnx_btree_node *first = root->children[0];
	assert(first->count == 3);
	assert(compare_pints(first->records[0]->key, data + 7) == 0);
	assert(compare_pints(first->records[1]->key, data + 3) == 0);
	assert(compare_pints(first->records[2]->key, data + 1) == 0);

	jnx_btree_node *second = root->children[1];
	assert(second->count == 3);
	assert(compare_pints(second->records[0]->key, data + 6) == 0);
	assert(compare_pints(second->records[1]->key, data + 0) == 0);
	assert(compare_pints(second->records[2]->key, data + 9) == 0);

	jnx_btree_node *third = root->children[2];
	assert(third->count == 2);
	assert(compare_pints(third->records[0]->key, data + 2) == 0);
	assert(compare_pints(third->records[1]->key, data + 5) == 0);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_growing_to_depth_of_3() {
	JNX_LOGC(JLOG_DEBUG,"- test_growing_to_depth_of_3: ");

	jnx_btree *tree = jnx_btree_create(2, compare_pints);
	int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	int i;
	for( i = 0; i < 10; i++ ) {
		jnx_btree_add(tree, data + i, data + i);
	}

	jnx_btree_node *root = tree->root;
	assert(root->count == 1);
	assert(root->is_leaf == 0);
	assert(root->records[1] == NULL && root->records[2] == NULL);
	assert(root->children[0] != NULL && root->children[1] != NULL);
	assert(root->children[2] == NULL && root->children[3] == NULL);
	assert(compare_pints(root->records[0]->key, data + 3) == 0);

	// Level 1 nodes
	jnx_btree_node *l1_0 = tree->root->children[0];
	assert(l1_0->count == 1);
	assert(l1_0->is_leaf == 0);
	assert(l1_0->records[1] == NULL && l1_0->records[2] == NULL);
	assert(l1_0->children[0] != NULL && l1_0->children[1] != NULL);
	assert(l1_0->children[2] == NULL && l1_0->children[3] == NULL);
	assert(compare_pints(l1_0->records[0]->key, data + 1) == 0);

	jnx_btree_node *l1_1 = tree->root->children[1];
	assert(l1_1->count == 2);
	assert(l1_1->is_leaf == 0);
	assert(l1_1->records[2] == NULL);
	assert(l1_1->children[0] != NULL && l1_1->children[1] != NULL && l1_1->children[2] != NULL);
	assert(l1_1->children[3] == NULL);
	assert(compare_pints(l1_1->records[0]->key, data + 5) == 0);
	assert(compare_pints(l1_1->records[1]->key, data + 7) == 0);

	// Level 2 leaf nodes
	jnx_btree_node *l2_0 = l1_0->children[0];
	assert(l2_0->count == 1);
	assert(l2_0->is_leaf == 1);
	assert(l2_0->records[1] == NULL && l2_0->records[2] == NULL);
	assert(l2_0->children[0] == NULL && l2_0->children[1] == NULL && l2_0->children[2] == NULL && l2_0->children[3] == NULL);
	assert(compare_pints(l2_0->records[0]->key, data + 0) == 0);

	jnx_btree_node *l2_1 = l1_0->children[1];
	assert(l2_1->count == 1);
	assert(l2_1->is_leaf == 1);
	assert(l2_1->records[1] == NULL && l2_1->records[2] == NULL);
	assert(l2_1->children[0] == NULL && l2_1->children[1] == NULL && l2_1->children[2] == NULL && l2_1->children[3] == NULL);
	assert(compare_pints(l2_1->records[0]->key, data + 2) == 0);

	jnx_btree_node *l2_2 = l1_1->children[0];
	assert(l2_2->count == 1);
	assert(l2_2->is_leaf == 1);
	assert(l2_2->records[1] == NULL && l2_2->records[2] == NULL);
	assert(l2_2->children[0] == NULL && l2_2->children[1] == NULL && l2_2->children[2] == NULL && l2_2->children[3] == NULL);
	assert(compare_pints(l2_2->records[0]->key, data + 4) == 0);

	jnx_btree_node *l2_3 = l1_1->children[1];
	assert(l2_3->count == 1);
	assert(l2_3->is_leaf == 1);
	assert(l2_3->records[1] == NULL && l2_3->records[2] == NULL);
	assert(l2_3->children[0] == NULL && l2_3->children[1] == NULL && l2_3->children[2] == NULL && l2_3->children[3] == NULL);
	assert(compare_pints(l2_3->records[0]->key, data + 6) == 0);

	jnx_btree_node *l2_4 = l1_1->children[2];
	assert(l2_4->count == 2);
	assert(l2_4->is_leaf == 1);
	assert(l2_4->records[2] == NULL);
	assert(l2_4->children[0] == NULL && l2_4->children[1] == NULL && l2_4->children[2] == NULL && l2_4->children[3] == NULL);
	assert(compare_pints(l2_4->records[0]->key, data + 8) == 0);
	assert(compare_pints(l2_4->records[1]->key, data + 9) == 0);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_splitting_inner_node() {
	JNX_LOGC(JLOG_DEBUG,"- test_splitting_inner_node: ");

	jnx_btree *tree = jnx_btree_create(2, compare_pints);
	int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

	int i;
	for( i = 0; i < 14; i++ ) {
		jnx_btree_add(tree, data + i, data + i);
	}

	jnx_btree_node *root = tree->root;
	assert(root->count == 2);
	assert(root->is_leaf == 0);
	assert(root->records[2] == NULL);
	assert(root->children[0] != NULL && root->children[1] != NULL && root->children[2] != NULL);
	assert(root->children[3] == NULL);
	assert(compare_pints(root->records[0]->key, data + 3) == 0);
	assert(compare_pints(root->records[1]->key, data + 7) == 0);

	// Level 1 nodes
	jnx_btree_node *l1_0 = tree->root->children[0];
	assert(l1_0->count == 1);
	assert(l1_0->is_leaf == 0);
	assert(l1_0->records[1] == NULL && l1_0->records[2] == NULL);
	assert(l1_0->children[0] != NULL && l1_0->children[1] != NULL);
	assert(l1_0->children[2] == NULL && l1_0->children[3] == NULL);
	assert(compare_pints(l1_0->records[0]->key, data + 1) == 0);

	jnx_btree_node *l1_1 = tree->root->children[1];
	assert(l1_1->count == 1);
	assert(l1_1->is_leaf == 0);
	assert(l1_1->records[1] == NULL && l1_1->records[2] == NULL);
	assert(l1_1->children[0] != NULL && l1_1->children[1] != NULL);
	assert(l1_1->children[2] == NULL && l1_1->children[3] == NULL);
	assert(compare_pints(l1_1->records[0]->key, data + 5) == 0);

	jnx_btree_node *l1_2 = tree->root->children[2];
	assert(l1_2->count == 2);
	assert(l1_2->is_leaf == 0);
	assert(l1_2->records[2] == NULL);
	assert(l1_2->children[0] != NULL && l1_2->children[1] != NULL && l1_2->children[2] != NULL);
	assert(l1_2->children[3] == NULL);
	assert(compare_pints(l1_2->records[0]->key, data + 9) == 0);
	assert(compare_pints(l1_2->records[1]->key, data + 11) == 0);

	// Level 2 leaf nodes
	jnx_btree_node *l2_0 = l1_0->children[0];
	assert(l2_0->count == 1);
	assert(l2_0->is_leaf == 1);
	assert(l2_0->records[1] == NULL && l2_0->records[2] == NULL);
	assert(l2_0->children[0] == NULL && l2_0->children[1] == NULL && l2_0->children[2] == NULL && l2_0->children[3] == NULL);
	assert(compare_pints(l2_0->records[0]->key, data + 0) == 0);

	jnx_btree_node *l2_1 = l1_0->children[1];
	assert(l2_1->count == 1);
	assert(l2_1->is_leaf == 1);
	assert(l2_1->records[1] == NULL && l2_1->records[2] == NULL);
	assert(l2_1->children[0] == NULL && l2_1->children[1] == NULL && l2_1->children[2] == NULL && l2_1->children[3] == NULL);
	assert(compare_pints(l2_1->records[0]->key, data + 2) == 0);

	jnx_btree_node *l2_2 = l1_1->children[0];
	assert(l2_2->count == 1);
	assert(l2_2->is_leaf == 1);
	assert(l2_2->records[1] == NULL && l2_2->records[2] == NULL);
	assert(l2_2->children[0] == NULL && l2_2->children[1] == NULL && l2_2->children[2] == NULL && l2_2->children[3] == NULL);
	assert(compare_pints(l2_2->records[0]->key, data + 4) == 0);

	jnx_btree_node *l2_3 = l1_1->children[1];
	assert(l2_3->count == 1);
	assert(l2_3->is_leaf == 1);
	assert(l2_3->records[1] == NULL && l2_3->records[2] == NULL);
	assert(l2_3->children[0] == NULL && l2_3->children[1] == NULL && l2_3->children[2] == NULL && l2_3->children[3] == NULL);
	assert(compare_pints(l2_3->records[0]->key, data + 6) == 0);

	jnx_btree_node *l2_4 = l1_2->children[0];
	assert(l2_4->count == 1);
	assert(l2_4->is_leaf == 1);
	assert(l2_4->records[1] == NULL && l2_4->records[2] == NULL);
	assert(l2_4->children[0] == NULL && l2_4->children[1] == NULL && l2_4->children[2] == NULL && l2_4->children[3] == NULL);
	assert(compare_pints(l2_4->records[0]->key, data + 8) == 0);

	jnx_btree_node *l2_5 = l1_2->children[1];
	assert(l2_5->count == 1);
	assert(l2_5->is_leaf == 1);
	assert(l2_5->records[1] == NULL && l2_5->records[2] == NULL);
	assert(l2_5->children[0] == NULL && l2_5->children[1] == NULL && l2_5->children[2] == NULL && l2_5->children[3] == NULL);
	assert(compare_pints(l2_5->records[0]->key, data + 10) == 0);

	jnx_btree_node *l2_6 = l1_2->children[2];
	assert(l2_6->count == 2);
	assert(l2_6->is_leaf == 1);
	assert(l2_6->records[2] == NULL);
	assert(l2_6->children[0] == NULL && l2_6->children[1] == NULL && l2_6->children[2] == NULL && l2_6->children[3] == NULL);
	assert(compare_pints(l2_6->records[0]->key, data + 12) == 0);
	assert(compare_pints(l2_6->records[1]->key, data + 13) == 0);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_lookup_in_empty_tree() {
	JNX_LOGC(JLOG_DEBUG,"- test_lookup_in_empty_tree: ");

	jnx_btree *tree = jnx_btree_create(2, compare_pints);
	int key = 42;

	void *val = jnx_btree_lookup(tree, (void *) &key);
	assert(val == NULL);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_lookup_in_single_record_tree() {
	JNX_LOGC(JLOG_DEBUG,"- test_lookup_in_single_record_tree: ");

	jnx_btree *tree = jnx_btree_create(2, compare_pints);
	int key = 42, key2 = -3;

	jnx_btree_add(tree, &key, &key);

	void *val = jnx_btree_lookup(tree, (void *) &key);
	assert(*((int *) val) == 42);

	val = jnx_btree_lookup(tree, &key2);
	assert(val == NULL);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_lookup_in_leaf_root() {
	JNX_LOGC(JLOG_DEBUG,"- test_insert_records_into_leaf_root: ");

	jnx_btree *tree = jnx_btree_create(5, compare_pints);
	int data[] = { 42, 12, 56, 3, 27, 100, 31, 1, 47 };
	int i;

	for ( i = 0; i < 9; i++ ) {
		jnx_btree_add(tree, data + i, data + i);
	}

	void *val = jnx_btree_lookup(tree, (void *) (data + 2));
	assert(*((int *) val) == 56);

	val = jnx_btree_lookup(tree, (void *) (data + 7));
	assert(*((int *) val) == 1);

	val = jnx_btree_lookup(tree, (void *) (data + 5));
	assert(*((int *) val) == 100);

	int bob = -1;
	val = jnx_btree_lookup(tree, (void *) &bob);
	assert(val == NULL);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_lookup_in_tree_of_depth_3() {
	JNX_LOGC(JLOG_DEBUG,"- test_lookup_in_tree_of_depth_3: ");

	jnx_btree *tree = jnx_btree_create(2, compare_pints);
	int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

	int i;
	for( i = 0; i < 14; i++ ) {
		jnx_btree_add(tree, data + i, data + i);
	}

	int *val = (int *) (jnx_btree_lookup(tree, data));
	assert(*val == 1);

	val = (int *) (jnx_btree_lookup(tree, data + 7));
	assert(*val == 8);

	val = (int *) (jnx_btree_lookup(tree, data + 13));
	assert(*val == 14);

	int bob = -1;
	val = (int *) (jnx_btree_lookup(tree, (void *) &bob));
	assert(val == NULL);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_lookup_does_not_modify_tree() {
	JNX_LOGC(JLOG_DEBUG,"- test_lookup_does_not_modify_tree: ");

	jnx_btree *tree = jnx_btree_create(2, compare_pints);
	int data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };

	int i;
	for( i = 0; i < 14; i++ ) {
		jnx_btree_add(tree, data + i, data + i);
	}

	int *val = (int *) (jnx_btree_lookup(tree, data));
	val = (int *) (jnx_btree_lookup(tree, data + 7));
	val = (int *) (jnx_btree_lookup(tree, data + 13));
	int bob = -1;
	val = (int *) (jnx_btree_lookup(tree, (void *) &bob));

	jnx_btree_node *root = tree->root;
	assert(root->count == 2);
	assert(root->is_leaf == 0);
	assert(root->records[2] == NULL);
	assert(root->children[0] != NULL && root->children[1] != NULL && root->children[2] != NULL);
	assert(root->children[3] == NULL);
	assert(compare_pints(root->records[0]->key, data + 3) == 0);
	assert(compare_pints(root->records[1]->key, data + 7) == 0);

	// Level 1 nodes
	jnx_btree_node *l1_0 = tree->root->children[0];
	assert(l1_0->count == 1);
	assert(l1_0->is_leaf == 0);
	assert(l1_0->records[1] == NULL && l1_0->records[2] == NULL);
	assert(l1_0->children[0] != NULL && l1_0->children[1] != NULL);
	assert(l1_0->children[2] == NULL && l1_0->children[3] == NULL);
	assert(compare_pints(l1_0->records[0]->key, data + 1) == 0);

	jnx_btree_node *l1_1 = tree->root->children[1];
	assert(l1_1->count == 1);
	assert(l1_1->is_leaf == 0);
	assert(l1_1->records[1] == NULL && l1_1->records[2] == NULL);
	assert(l1_1->children[0] != NULL && l1_1->children[1] != NULL);
	assert(l1_1->children[2] == NULL && l1_1->children[3] == NULL);
	assert(compare_pints(l1_1->records[0]->key, data + 5) == 0);

	jnx_btree_node *l1_2 = tree->root->children[2];
	assert(l1_2->count == 2);
	assert(l1_2->is_leaf == 0);
	assert(l1_2->records[2] == NULL);
	assert(l1_2->children[0] != NULL && l1_2->children[1] != NULL && l1_2->children[2] != NULL);
	assert(l1_2->children[3] == NULL);
	assert(compare_pints(l1_2->records[0]->key, data + 9) == 0);
	assert(compare_pints(l1_2->records[1]->key, data + 11) == 0);

	// Level 2 leaf nodes
	jnx_btree_node *l2_0 = l1_0->children[0];
	assert(l2_0->count == 1);
	assert(l2_0->is_leaf == 1);
	assert(l2_0->records[1] == NULL && l2_0->records[2] == NULL);
	assert(l2_0->children[0] == NULL && l2_0->children[1] == NULL && l2_0->children[2] == NULL && l2_0->children[3] == NULL);
	assert(compare_pints(l2_0->records[0]->key, data + 0) == 0);

	jnx_btree_node *l2_1 = l1_0->children[1];
	assert(l2_1->count == 1);
	assert(l2_1->is_leaf == 1);
	assert(l2_1->records[1] == NULL && l2_1->records[2] == NULL);
	assert(l2_1->children[0] == NULL && l2_1->children[1] == NULL && l2_1->children[2] == NULL && l2_1->children[3] == NULL);
	assert(compare_pints(l2_1->records[0]->key, data + 2) == 0);

	jnx_btree_node *l2_2 = l1_1->children[0];
	assert(l2_2->count == 1);
	assert(l2_2->is_leaf == 1);
	assert(l2_2->records[1] == NULL && l2_2->records[2] == NULL);
	assert(l2_2->children[0] == NULL && l2_2->children[1] == NULL && l2_2->children[2] == NULL && l2_2->children[3] == NULL);
	assert(compare_pints(l2_2->records[0]->key, data + 4) == 0);

	jnx_btree_node *l2_3 = l1_1->children[1];
	assert(l2_3->count == 1);
	assert(l2_3->is_leaf == 1);
	assert(l2_3->records[1] == NULL && l2_3->records[2] == NULL);
	assert(l2_3->children[0] == NULL && l2_3->children[1] == NULL && l2_3->children[2] == NULL && l2_3->children[3] == NULL);
	assert(compare_pints(l2_3->records[0]->key, data + 6) == 0);

	jnx_btree_node *l2_4 = l1_2->children[0];
	assert(l2_4->count == 1);
	assert(l2_4->is_leaf == 1);
	assert(l2_4->records[1] == NULL && l2_4->records[2] == NULL);
	assert(l2_4->children[0] == NULL && l2_4->children[1] == NULL && l2_4->children[2] == NULL && l2_4->children[3] == NULL);
	assert(compare_pints(l2_4->records[0]->key, data + 8) == 0);

	jnx_btree_node *l2_5 = l1_2->children[1];
	assert(l2_5->count == 1);
	assert(l2_5->is_leaf == 1);
	assert(l2_5->records[1] == NULL && l2_5->records[2] == NULL);
	assert(l2_5->children[0] == NULL && l2_5->children[1] == NULL && l2_5->children[2] == NULL && l2_5->children[3] == NULL);
	assert(compare_pints(l2_5->records[0]->key, data + 10) == 0);

	jnx_btree_node *l2_6 = l1_2->children[2];
	assert(l2_6->count == 2);
	assert(l2_6->is_leaf == 1);
	assert(l2_6->records[2] == NULL);
	assert(l2_6->children[0] == NULL && l2_6->children[1] == NULL && l2_6->children[2] == NULL && l2_6->children[3] == NULL);
	assert(compare_pints(l2_6->records[0]->key, data + 12) == 0);
	assert(compare_pints(l2_6->records[1]->key, data + 13) == 0);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "OK\n");
}

void test_removing_key_from_empty_tree() {
	JNX_LOGC(JLOG_DEBUG,"- test_removing_key_from_empty_tree:");

	jnx_btree *tree = NULL;
	int kv = 42;

	jnx_btree_remove(tree, (void *) &kv, NULL, NULL);
	assert(tree == NULL);

	tree = jnx_btree_create(2, compare_pints);
	jnx_btree_remove(tree, (void *) &kv, NULL, NULL);
	assert(tree->root->count == 0);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}

void test_removing_record_from_single_record_tree() {
	JNX_LOGC(JLOG_DEBUG,"- test_removing_record_from_single_record_tree:");

	jnx_btree *tree = NULL;
	int kv = 42, kv2 = 24;

	tree = jnx_btree_create(2, compare_pints);
	jnx_btree_add(tree, (void *) &kv, (void *) &kv);
	assert(tree->root->count == 1);

	jnx_btree_remove(tree, (void *) &kv2, NULL, NULL);
	assert(tree->root->count == 1);
	assert(compare_pints(tree->root->records[0]->key, (void *) &kv) == 0);

	jnx_btree_remove(tree, (void *) &kv, NULL, NULL);
	assert(tree->root->count == 0);
	assert(tree->root->records[0] == NULL);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}

void test_removing_record_from_leaf_root() {
	JNX_LOGC(JLOG_DEBUG,"- test_removing_record_from_leaf_root:");

	jnx_btree *tree = jnx_btree_create(5, compare_pints);
	int data[] = { 42, 12, 56, 3, 27, 100, 31, 1, 47 };
	int i;

	for ( i = 0; i < 9; i++ ) {
		jnx_btree_add(tree, data + i, data + i);
	}

	// remove from the middle
	jnx_btree_remove(tree, (void *) data, NULL, NULL);
	assert(tree->root->count == 8);
	char *contents = int_node_contents(tree->root);
	assert(strcmp(contents, "1 3 12 27 31 47 56 100 ") == 0);
	free(contents);

	// remove first
	jnx_btree_remove(tree, (void *) (data + 7), NULL, NULL);
	assert(tree->root->count == 7);
	contents = int_node_contents(tree->root);
	assert(strcmp(contents, "3 12 27 31 47 56 100 ") == 0);
	free(contents);

	// remove last
	jnx_btree_remove(tree, (void *) (data + 5), NULL, NULL);
	assert(tree->root->count == 6);
	contents = int_node_contents(tree->root);
	assert(strcmp(contents, "3 12 27 31 47 56 ") == 0);
	free(contents);

	// remove record not in node
	int bob = -1;
	jnx_btree_remove(tree, (void *) &bob, NULL, NULL);
	assert(tree->root->count == 6);
	contents = int_node_contents(tree->root);
	assert(strcmp(contents, "3 12 27 31 47 56 ") == 0);
	free(contents);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}

void print_char_tree_at_node(jnx_btree_node *node, char *(*str)(jnx_btree_node *), int level) {
	int i;

	if ( node->is_leaf ) {
		for ( i = 0; i < level; i++ ) {
			JNX_LOGC(JLOG_DEBUG,"    ");
		}
		char *temp = str(node);
		JNX_LOGC(JLOG_DEBUG,"%s\n", temp);
		free(temp);

		return;
	}

	for ( i = 0; i <= node->count; i++ ) {
		print_char_tree_at_node(node->children[i], str, level + 1);

		if ( i < node->count ) {
			int j;
			for ( j = 0; j < level; j++ ) {
				JNX_LOGC(JLOG_DEBUG,"    ");
			}

			char *pc = (char *) node->records[i]->key;
			JNX_LOGC(JLOG_DEBUG,"%c\n", *pc);
		}
	}
}

void populate_char_tree(jnx_btree *tree, char *data, int size, int random, int midway_split) {
	int i;

	if ( random ) {
		srand(time(0));

		int* flags = calloc(size, sizeof(int));
		for ( i = 0; i < size; i++ ) {
			int next;
			do {
				next = rand() % size;
			} while ( *(flags + next) == 1 );

			flags[next] = 1;
			jnx_btree_add(tree, (void *) (data + next), (void *) (data + next));
		}

		free(flags);
	} else {
		for ( i = 0; i < size; i++ ) {
			if ( midway_split ) {
				int j = (11 + i) % size;
				jnx_btree_add(tree, (void *) (data + j), (void *) (data + j));
			} else {
				jnx_btree_add(tree, (void *) (data + i), (void *) (data + i));
			}
		}
	}
}

jnx_btree *build_alphabet_tree(int random, int midway_split) {
	jnx_btree *tree = jnx_btree_create(3, compare_pchars);

	char *data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	populate_char_tree(tree, data, 26, random, midway_split);

	return tree;
}

jnx_btree *build_mixed_tree(int random, int midway_split) {
	jnx_btree *tree = jnx_btree_create(2, compare_pchars);

	char *data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789AB18CD23EFB614";

	populate_char_tree(tree, data, 50, random, midway_split);

	return tree;
}

void test_mixed_tree() {
	JNX_LOGC(JLOG_DEBUG,"- test_mixed_tree:\n");

	// Try reverse order insertion
	JNX_LOGC(JLOG_DEBUG,"\n    Midway Mixed Insertion\n");
	JNX_LOGC(JLOG_DEBUG,"    ----------------------\n");

	jnx_btree *tree = build_mixed_tree(0, 1);

	print_char_tree_at_node(tree->root, char_node_contents, 1);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}

void test_alphabet_tree() {
	JNX_LOGC(JLOG_DEBUG,"- test_alphabet_tree:");

	// Try in order insertion
	jnx_node *n;
	char c;
	jnx_btree *tree = build_alphabet_tree(0, 0);
	jnx_list *keys = jnx_list_create();
	jnx_btree_keys(tree, keys);

	for ( n = keys->head, c = 'A'; n != NULL; n = n->next_node, c++ )
		assert(c == (char)(*((char *)(n->_data))));

	jnx_list_destroy(&keys);
	jnx_btree_destroy(tree);

	// Try midway insertion
	tree = build_alphabet_tree(0, 1);
	keys = jnx_list_create();
	jnx_btree_keys(tree, keys);

	for ( n = keys->head, c = 'A'; n != NULL; n = n->next_node, c++ )
		assert(c == (char)(*((char *)(n->_data))));

	jnx_list_destroy(&keys);
	jnx_btree_destroy(tree);

	// Try random insertion
	tree = build_alphabet_tree(1, 0);
	keys = jnx_list_create();
	jnx_btree_keys(tree, keys);

	for ( n = keys->head, c = 'A'; n != NULL; n = n->next_node, c++ )
		assert(c == (char)(*((char *)(n->_data))));

	jnx_list_destroy(&keys);
	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}

void test_simple_remove_from_leaf() {
	JNX_LOGC(JLOG_DEBUG,"- test_simple_remove_from_leaf:");

	jnx_btree *tree = build_alphabet_tree(0, 0);

	char c = 'X';

	jnx_btree_node *leaf = tree->root->children[1]->children[4];
	assert(leaf->count == 5);

	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	char *contents = char_node_contents(leaf);
	assert(strcmp(contents, "VWYZ") == 0);
	assert(leaf->count == 4);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);

	free(contents);
	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}

int strcmp_node_contents_with_string(jnx_btree_node *node, char *str) {
	char *contents = char_node_contents(node);
	int retval = strcmp(contents, str);
	free(contents);
	return retval;
}

void test_removing_record_from_inner_node() {
	JNX_LOGC(JLOG_DEBUG,"- test_removing_record_from_inner_node:");

	jnx_btree *tree;
	char c;
	jnx_btree_node *root;

	// Case when preceeding sibling has degree >= n
	tree = build_alphabet_tree(0, 1);
	c = 'N';

	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);

	root = tree->root;
	assert(strcmp_node_contents_with_string(root->children[0], "CFIM") == 0);
	assert(root->children[0]->count == 4);
	assert(root->children[0]->records[4] == NULL);

	assert(strcmp_node_contents_with_string(root->children[0]->children[3], "JKL") == 0);
	assert(root->children[0]->children[3]->count == 3);
	assert(root->children[0]->children[3]->records[3] == NULL);

	jnx_btree_destroy(tree);

	// Case when succeeding sibling has degree >= n
	tree = build_alphabet_tree(0, 1);
	c = 'I';

	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);

	root = tree->root;
	assert(strcmp_node_contents_with_string(root->children[0], "CFJN") == 0);
	assert(root->children[0]->count == 4);
	assert(root->children[0]->records[4] == NULL);

	assert(strcmp_node_contents_with_string(root->children[0]->children[3], "KLM") == 0);
	assert(root->children[0]->children[3]->count == 3);
	assert(root->children[0]->children[3]->records[3] == NULL);

	jnx_btree_destroy(tree);

	// Case when siblings have to be merged, i.e. both have degree < n
	tree = build_alphabet_tree(0, 1);
	c = 'F';

	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);

	root = tree->root;
	assert(strcmp_node_contents_with_string(root->children[0], "CIN") == 0);
	assert(root->children[0]->count == 3);
	assert(root->children[0]->records[3] == NULL);

	assert(strcmp_node_contents_with_string(root->children[0]->children[1], "DEGH") == 0);
	assert(root->children[0]->children[1]->count == 4);
	assert(root->children[0]->children[1]->records[4] == NULL);

	jnx_btree_destroy(tree);

	// Case when siblings have to be merged but on a boundary
	tree = build_alphabet_tree(0, 1);
	c = 'C';

	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);

	root = tree->root;
	assert(strcmp_node_contents_with_string(root->children[0], "FIN") == 0);
	assert(root->children[0]->count == 3);
	assert(root->children[0]->records[3] == NULL);

	assert(strcmp_node_contents_with_string(root->children[0]->children[0], "ABDE") == 0);
	assert(root->children[0]->children[0]->count == 4);
	assert(root->children[0]->children[0]->records[4] == NULL);

	jnx_btree_destroy(tree);

	tree = build_alphabet_tree(0, 1);
	c = 'x';

	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	root = tree->root;
	assert(strcmp_node_contents_with_string(root, "N") == 0);
	assert(root->count == 1);
	assert(strcmp_node_contents_with_string(root->children[1], "QTW") == 0);
	assert(root->children[1]->count == 3);
	assert(strcmp_node_contents_with_string(root->children[1]->children[3], "XYZ") == 0);
	assert(root->children[1]->children[3]->count == 3);

	c = '1';

	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	root = tree->root;
	assert(strcmp_node_contents_with_string(root, "N") == 0);
	assert(root->count == 1);
	assert(strcmp_node_contents_with_string(root->children[0], "FI") == 0);
	assert(root->children[0]->count == 2);
	assert(strcmp_node_contents_with_string(root->children[0]->children[0], "ABCDE") == 0);
	assert(root->children[0]->children[0]->count == 5);

	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}

void test_removing_key_from_child_that_has_too_few_records() {
	JNX_LOGC(JLOG_DEBUG,"- test_removing_key_from_child_that_has_too_few_records:");

	jnx_btree *tree = build_mixed_tree(0, 1);
	char c = 'D';
	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	jnx_btree_node *root = tree->root;
	assert(strcmp_node_contents_with_string(root->children[1], "BH") == 0);
	assert(root->children[1]->count == 2);
	assert(strcmp_node_contents_with_string(root->children[1]->children[0], "9") == 0);
	assert(root->children[1]->children[0]->count == 1);
	assert(strcmp_node_contents_with_string(root->children[1]->children[1], "F") == 0);
	assert(root->children[1]->children[1]->count == 1);
	assert(strcmp_node_contents_with_string(root->children[1]->children[2], "JM") == 0);
	assert(root->children[1]->children[2]->count == 2);
	assert(strcmp_node_contents_with_string(root->children[1]->children[1]->children[0], "CE") == 0);
	assert(root->children[1]->children[1]->children[0]->count == 2);
	assert(strcmp_node_contents_with_string(root->children[1]->children[1]->children[1], "G") == 0);
	assert(root->children[1]->children[1]->children[1]->count == 1);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);
	//    JNX_LOGC(JLOG_DEBUG,"----------------------------------------------\n");
	jnx_btree_destroy(tree);

	tree = build_mixed_tree(0, 1);
	c = 'S';
	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	root = tree->root;
	assert(strcmp_node_contents_with_string(root, "7F") == 0);
	assert(root->count == 2);
	assert(strcmp_node_contents_with_string(root->children[1], "B") == 0);
	assert(root->children[1]->count == 1);
	assert(strcmp_node_contents_with_string(root->children[2], "OT") == 0);
	assert(root->children[2]->count == 2);
	assert(strcmp_node_contents_with_string(root->children[2]->children[0], "HJM") == 0);
	assert(root->children[2]->children[0]->count == 3);
	assert(strcmp_node_contents_with_string(root->children[2]->children[1], "Q") == 0);
	assert(root->children[2]->children[1]->count == 1);
	assert(strcmp_node_contents_with_string(root->children[2]->children[2], "W") == 0);
	assert(root->children[2]->children[2]->count == 1);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);
	//    JNX_LOGC(JLOG_DEBUG,"----------------------------------------------\n");
	jnx_btree_destroy(tree);

	tree = build_mixed_tree(0, 1);
	c = '9';
	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	root = tree->root;
	assert(strcmp_node_contents_with_string(root->children[1], "F") == 0);
	assert(root->children[1]->count == 1);
	assert(strcmp_node_contents_with_string(root->children[1]->children[0], "BD") == 0);
	assert(root->children[1]->children[0]->count == 2);
	assert(strcmp_node_contents_with_string(root->children[1]->children[1], "HJM") == 0);
	assert(root->children[1]->children[1]->count == 3);
	assert(strcmp_node_contents_with_string(root->children[1]->children[0]->children[0], "8A") == 0);
	assert(root->children[1]->children[0]->children[0]->count == 2);
	assert(strcmp_node_contents_with_string(root->children[1]->children[0]->children[1], "C") == 0);
	assert(root->children[1]->children[0]->children[1]->count == 1);
	assert(strcmp_node_contents_with_string(root->children[1]->children[0]->children[2], "E") == 0);
	assert(root->children[1]->children[0]->children[2]->count == 1);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);
	//    JNX_LOGC(JLOG_DEBUG,"----------------------------------------------\n");
	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}

void test_removing_key_from_root() {
	JNX_LOGC(JLOG_DEBUG,"- test_removing_key_from_root:");

	jnx_btree *tree = build_alphabet_tree(0, 1);
	char c = 'Q';

	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	jnx_btree_node *root = tree->root;
	assert(strcmp_node_contents_with_string(root, "P") == 0);
	assert(root->count == 1);
	assert(strcmp_node_contents_with_string(root->children[0], "CFIM") == 0);
	assert(root->children[0]->count == 4);
	assert(strcmp_node_contents_with_string(root->children[0]->children[3], "JKL") == 0);
	assert(root->children[0]->children[3]->count == 3);
	assert(strcmp_node_contents_with_string(root->children[0]->children[4], "NO") == 0);
	assert(root->children[0]->children[4]->count == 2);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);
	//    JNX_LOGC(JLOG_DEBUG,"----------------------------------------------\n");
	jnx_btree_destroy(tree);

	tree = build_mixed_tree(0, 1);
	c = '7';

	jnx_btree_remove(tree, (void *) &c, NULL, NULL);

	root = tree->root;
	assert(strcmp_node_contents_with_string(root, "8O") == 0);
	assert(root->count == 2);
	assert(strcmp_node_contents_with_string(root->children[1], "F") == 0);
	assert(root->children[1]->count == 1);
	assert(strcmp_node_contents_with_string(root->children[1]->children[0], "BD") == 0);
	assert(root->children[1]->children[0]->count == 2);
	assert(strcmp_node_contents_with_string(root->children[1]->children[0]->children[0], "9A") == 0);
	assert(root->children[1]->children[0]->children[0]->count == 2);

	//    print_char_tree_at_node(tree->root, char_node_contents, 1);
	//    JNX_LOGC(JLOG_DEBUG,"----------------------------------------------\n");
	jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}

int comparo(void *key1, void *key2) {
	return strcmp((char *)key1, (char *)key2);
}

void test_tree_level_1_add_and_remove() {
	JNX_LOGC(JLOG_DEBUG,"- test_tree_level_1_add_and_remove:");

	char *guids[10];
	guids[0] = "84203222311136641991772051085517620033121";
	guids[1] = "502443080732277718015213148367468529";
	guids[2] = "225671272266637901692418621158100228";
	guids[3] = "14498619620023367185157152325168237208195";
	guids[4] = "362115127142247715912917232741442052548";
	guids[5] = "250201131217183204761341544314863981895992";
	guids[6] = "14210254161789709118821423114742294686";
	guids[7] = "1199207441132407112617930104101119139283";
	guids[8] = "87729724579207718916710744983217249235";
	guids[9] = "18715518822724720669159182182841452103209";

	jnx_btree *tree = jnx_btree_create(sizeof(int), comparo);

	int i;
	for ( i = 0; i < 10; i++ ) {
		void *next = (void *)(guids[i]);
//		printf("ADD -> %s\n", (char *)next);
		jnx_btree_add(tree, next, next);		
	}

	jnx_list *keys = jnx_list_create();
	jnx_btree_keys(tree, keys);
    
	while(keys->head) {
        char *a_key = keys->head->_data;
     
	 	void *val = jnx_btree_lookup(tree, (void *) a_key);
   		void *key_h;
		void *value_h;
		jnx_btree_remove(tree, a_key, &key_h, &value_h);  
//	    printf("REMOVE -> %s\n",(char*)(key_h));
		keys->head = keys->head->next_node;
    }
    jnx_list_destroy(&keys);
    jnx_btree_destroy(tree);

	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}

int main() {
	//	JNX_LOGC(JLOG_DEBUG,"[DEBUG] sizeof(record) = %u\n", sizeof(record));
	//	JNX_LOGC(JLOG_DEBUG,"[DEBUG] sizeof(jnx_btree_node) = %u\n", sizeof(jnx_btree_node));
	//	JNX_LOGC(JLOG_DEBUG,"[DEBUG] sizeof(jnx_btree) = %u\n", sizeof(jnx_btree));

	JNX_LOGC(JLOG_DEBUG,"Running B-tree tests...\n");

	// Insertion tests
	test_new_node();
	test_new_empty_tree();
	test_insert_first_record_into_tree();
	test_insert_records_into_leaf_root();
	test_growing_to_depth_of_2();
	test_spliting_a_leaf_node_that_is_not_root();
	test_growing_to_depth_of_3();
	test_splitting_inner_node();
	test_alphabet_tree();

	// Lookup tests
	test_lookup_in_empty_tree();
	test_lookup_in_single_record_tree();
	test_lookup_in_leaf_root();
	test_lookup_in_tree_of_depth_3();
	test_lookup_does_not_modify_tree();

	// Remove tests
	test_removing_key_from_empty_tree();
	test_removing_record_from_single_record_tree();
	test_removing_record_from_leaf_root();
	test_simple_remove_from_leaf();
	test_removing_record_from_inner_node();
	//    test_mixed_tree();
	test_removing_key_from_child_that_has_too_few_records();
	test_removing_key_from_root();
	test_tree_level_1_add_and_remove();

	JNX_LOGC(JLOG_DEBUG,"B-tree tests completed.\n");

	return 0;
}
