/*
 * =====================================================================================
 *
 *       Filename:  jnxbtree.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/10/14 22:14:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (draganglumac), dragan.glumac@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include "jnxbtree.h"
#include "jnxcheck.h"
#include "jnxtypes.h"

jnx_btree_node *new_node(jnx_int32 order, int32_t is_leaf) {
  jnx_btree_node *node = calloc(1, sizeof(jnx_btree_node));

  node->count = 0; // just to be explicit, calloc will already zero the field
  node->is_leaf = is_leaf;
  node->records = calloc(2 * order, sizeof(record*));
  node->children = calloc(2 * order + 1, sizeof(jnx_btree_node*));

  return node;
}

void delete_node(jnx_btree_node *node) {
  free(node->records);
  free(node->children);
  free(node);
}

void move_contents_from_index(jnx_btree_node *source, jnx_btree_node *target, jnx_int32 index) {
  // Copy the RHS half of the records and children to new node
  memmove((void *) target->records,
      (const void*) (source->records + index),
      (source->count / 2) * sizeof(record *));
  memmove((void *) target->children,
      (const void*) (source->children + index),
      (source->count / 2 + 1) * sizeof(jnx_btree_node *));

  // Zero out the old records and children in the old node
  jnx_int32 new_count = source->count / 2;
  bzero((void *) (source->records + index - 1), (new_count + 1) * sizeof(record *));
  bzero((void *) (source->children + index), (new_count + 1) * sizeof(jnx_btree_node *));

  // Adjust record counts in the nodes
  source->count = new_count;
  target->count = new_count;
}

void shift_records_right_from_index(jnx_btree_node *node, jnx_int32 index) {
  memmove((void *)(node->records + (index + 1)),
      (const void*)(node->records + index),
      (node->count - index) * sizeof(record *));
}

void shift_children_right_from_index(jnx_btree_node *node, jnx_int32 index) {
  memmove((void *)(node->children + (index + 1)),
      (const void*)(node->children + index),
      (node->count + 1 - index) * sizeof(jnx_btree_node *));
}

void shift_records_left_from_index(jnx_btree_node *node, jnx_int32 index) {
  memmove((void *)(node->records + index - 1),
      (const void*)(node->records + index),
      (node->count - index) * sizeof(record *));
  bzero((void *) (node->records + node->count - 1), sizeof(record *));
}

void shift_children_left_from_index(jnx_btree_node *node, jnx_int32 index) {
  memmove((void *)(node->children + index - 1),
      (const void*)(node->children + index),
      (node->count - index + 1) * sizeof(jnx_btree_node *));
  bzero((void *) (node->children + node->count), sizeof(jnx_btree_node *));
}

jnx_int32 is_node_full(jnx_btree *tree, jnx_btree_node *node) {
  if ( node->count == (2 * tree->order - 1) ) {
    return 1;
  }

  return 0;
}

jnx_int32 is_node_empty(jnx_btree_node *node) {
  if ( node == NULL || node->count == 0 ) {
    return 1;
  }

  return 0;
}

jnx_int32 find_index_for_record(jnx_btree *tree, jnx_btree_node *node, record *r) {
  // Records are ordered so to make search efficient
  // (especially for arrays with lots of records)
  // we use binary chop, which is O(lg n) time.

  if ( node->count == 0 ) {
    return 0;
  }

  jnx_int32 left_bound = 0;
  jnx_int32 right_bound = node->count - 1;
  jnx_int32 curr_index = (right_bound - left_bound) / 2;

  do {
    void *key = node->records[curr_index]->key;
    jnx_int32 cf = tree->compare_function(key, r->key);

    if ( cf == 0 ) {
      // Found exact match
      return curr_index;
    }

    if ( cf < 0 ) {
      left_bound = curr_index + 1;
    } else {
      right_bound = curr_index - 1;
    }

    curr_index = left_bound + (right_bound - left_bound) / 2;
  } while ( right_bound >= left_bound );

  return curr_index;
}

void split_child_at_index(jnx_btree *tree, jnx_btree_node *node, jnx_int32 child_index) {
  jnx_int32 tree_order = tree->order;

  jnx_btree_node *temp = node->children[child_index];
  jnx_btree_node *sibling = new_node(tree_order, temp->is_leaf);
  record *middle = temp->records[tree_order - 1];

  move_contents_from_index(temp, sibling, tree_order);

  // Now rearrange "node" to fit the new record and its children
  jnx_int32 i = find_index_for_record(tree, node, middle);
  if ( node->records[i] != NULL ) {
    // Shift only if the record slot is not empty
    shift_records_right_from_index(node, i);
    shift_children_right_from_index(node, i + 1);
  }

  // move the middle record up
  node->records[i] = middle;
  // attach newly created sibling
  node->children[i + 1] = sibling;
  // finally, increment the record count in the current node
  node->count++;
}

void add_record_to_non_full_leaf(jnx_btree *tree, jnx_btree_node *node, record *r) {
  jnx_int32 i = find_index_for_record(tree, node, r);

  if ( node->records[i] == NULL ) {
    node->records[i] = r;
    node->count++;
  } else {
    void *key = node->records[i]->key;

    if ( tree->compare_function(key, r->key) == 0 ) {
      // Replace the value only scenario
      free(node->records[i]);
      node->records[i] = r;
    } else {
      shift_records_right_from_index(node, i);
      node->records[i] = r;
      node->count++;
    }
  }
}

/*
 * To guarantee that leaf node is never full, we split any full
 * node on the way down the tree, find the appropriate subtree
 * to traverse, and insert the node jnx_int32o the subtree.
 *
 * The only special case is if the root is a leaf node. We handle
 * this in jnx_btree_add API function.
 */
void insert_into_tree_at_node(jnx_btree *tree, jnx_btree_node *node, record *r) {
  if ( node->is_leaf ) {
    add_record_to_non_full_leaf(tree, node, r);
    return;
  }

  jnx_int32 i = find_index_for_record(tree, node, r);

  if ( i < node->count ) {
    if ( tree->compare_function(node->records[i]->key, r->key) == 0 ) {
      // Same key so just replace the old record with the new one
      free(node->records[i]);
      node->records[i] = r;
      return;
    }
  }

  if ( is_node_full(tree, node->children[i]) ) {
    split_child_at_index(tree, node, i);

    if ( tree->compare_function(node->records[i]->key, r->key) == 0 ) {
      // Case when the node that moved up is actually the node we want to insert
      free(node->records[i]);
      node->records[i] = r;
      return;
    }

    if ( tree->compare_function(node->records[i]->key, r->key) < 0 ) {
      // We're going to the right of the record that moved up
      i++;
    }
  }

  // Recurse down jnx_int32o the appropriate subtree
  insert_into_tree_at_node(tree, node->children[i], r);
}

void delete_node_and_subtrees(jnx_btree_node *node) {
  if ( node == NULL ) {
    return;
  }

  // Delete all the children first
  jnx_int32 i;
  for ( i = 0; i <= node->count; i++ ) {
    if ( node->children[i] != NULL ) {
      delete_node_and_subtrees(node->children[i]);
    }
  }

  // Free memory for all the records
  for (i = 0; i < node->count; i++ ) {
    if ( node->records[i] != NULL ) {
      free(node->records[i]);
    }
  }

  // Finally, free the node itself
  delete_node(node);
}

void *find_value_for_key_in_node(jnx_btree *tree, jnx_btree_node *node, void *key) {
  if ( is_node_empty(node) ) {
    return NULL;
  }

  record r;
  r.key = key;

  jnx_int32 i =  find_index_for_record(tree, node, &r);

  if ( node->records[i] != NULL ) {
    if ( tree->compare_function(r.key, node->records[i]->key) == 0 ) {
      return node->records[i]->value;
    }
  }

  return find_value_for_key_in_node(tree, node->children[i], key);
}

record *find_rightmost_record_in_subtree_at_node(jnx_btree_node *node) {
  jnx_btree_node *temp = node;

  while ( temp->children[temp->count] != NULL ) {
    temp = temp->children[temp->count];
  }

  return temp->records[temp->count - 1];
}

record *find_leftmost_record_in_subtree_at_node(jnx_btree_node *node) {
  jnx_btree_node *temp = node;

  while ( temp->children[0] != NULL ) {
    temp = temp->children[0];
  }

  return temp->records[0];
}

void merge_subtrees_around_index(jnx_btree *tree, jnx_btree_node *node, jnx_int32 index) {
  jnx_btree_node *root = tree->root;

  jnx_btree_node *first = node->children[index];
  jnx_btree_node *second = node->children[index + 1];

  first->records[first->count] = node->records[index];
  first->count++;

  memmove((void *)(first->records + first->count),
      (const void *)(second->records),
      second->count * sizeof(record *));

  memmove((void *)(first->children + first->count),
      (const void *)(second->children),
      (second->count + 1) * sizeof(jnx_btree_node *));

  // Adjust the count
  first->count += second->count;

  // Fill the gap in node by shifting records and children
  // that are to the right of index by 1 position to the left.
  node->children[index + 1] = first;
  shift_records_left_from_index(node, index + 1);
  shift_children_left_from_index(node, index + 2);

  // Removed the record from node so adjust the count
  node->count--;

  // Delete the second node, i.e. free its memory on the heap
  delete_node(second);

  // Since root is the only node that can have less than t-1 records
  // we have to account for the fact when root has only one record
  // and that record's just been pushed down to the first child as a
  // result of the merge.
  if ( node == root ) {
    tree->root = first;
    delete_node(root);
  }
}

record *delete_record_from_node(jnx_btree *tree, jnx_btree_node *node, record *r) {
  jnx_int32 i = find_index_for_record(tree, node, r);

  if ( node->is_leaf ) {
    if ( node == tree->root || node->count >= tree->order) {
      if ( i < node->count ) {
        if ( tree->compare_function(node->records[i]->key, r->key) == 0 ) {
          record *temp = node->records[i];
          shift_records_left_from_index(node, i + 1);
          node->count--;

          return temp;
        }
      }
    }

    return NULL;
  }

  // Index 'i' returned by find_index_for_record is one of two things:
  // - index of the record itself, if the key is in this node
  // - index of the appropriate sutbree, if key is not in this node
  // This unfortunately means that we have to guard against going past the
  // records array upper bound which is at most node->count + 1
  jnx_int32 rec_i = i < node->count ? i : node->count - 1;
  record *node_rec = node->records[rec_i];
  record *temp = NULL, *retval = NULL;

  if ( tree->compare_function(node_rec->key, r->key) == 0 ) {
    // It is safe not to check for NULL children here because
    // we are in an internal node, and since by definition
    // a B-tree grows from the leaves up, and the only way to
    // add a level is to split a node at the previous, a record
    // in an inner node will have both left and right subtrees.

    if ( node->children[rec_i]->count >= tree->order ) {
      temp = find_rightmost_record_in_subtree_at_node(node->children[rec_i]);

      node->records[rec_i] = malloc(sizeof(record));
      node->records[rec_i]->key = temp->key;
      node->records[rec_i]->value = temp->value;

      retval = delete_record_from_node(tree, node->children[rec_i], temp);

      free(node_rec);
    } else if ( node->children[rec_i + 1]->count >= tree->order ) {
      temp = find_leftmost_record_in_subtree_at_node(node->children[rec_i + 1]);

      node->records[rec_i] = malloc(sizeof(record));
      node->records[rec_i]->key = temp->key;
      node->records[rec_i]->value = temp->value;

      retval = delete_record_from_node(tree, node->children[rec_i + 1], temp);

      free(node_rec);
    } else {
      merge_subtrees_around_index(tree, node, rec_i);
      retval = delete_record_from_node(tree, node->children[rec_i], r);
    }

    return retval;
  }

  if ( node->children[i]->count < tree->order ) {
    // guard the boundaries
    jnx_int32 b = i > 0 ? i - 1 : 0;
    jnx_int32 a = i < node->count ? i + 1 : node->count;

    jnx_btree_node *subtree = node->children[i];

    if ( a != i && node->children[a]->count >= tree->order ) {
      jnx_btree_node *sibling = node->children[a];

      // Shift records around
      subtree->records[subtree->count] = node->records[i];
      node->records[i] = sibling->records[0];
      subtree->children[subtree->count + 1] = sibling->children[0];

      // Fix up sibling
      shift_records_left_from_index(sibling, 1);
      shift_children_left_from_index(sibling, 1);

      // Adjust counts
      subtree->count++;
      sibling->count--;
    } else if ( b != i && node->children[b]->count >= tree->order ) {
      jnx_btree_node *sibling = node->children[b];
      jnx_int32 rec_i = i < node->count ? i : node->count - 1;

      // Shift records around
      shift_records_right_from_index(subtree, 0);
      shift_children_right_from_index(subtree, 0);
      subtree->records[0] = node->records[rec_i];
      node->records[rec_i] = sibling->records[sibling->count - 1];
      subtree->children[0] = sibling->children[sibling->count];

      // Fix up the sibling
      sibling->records[sibling->count - 1] = NULL;
      sibling->children[sibling->count] = NULL;

      // Adjust counts
      subtree->count++;
      sibling->count--;
    } else {
      merge_subtrees_around_index(tree, node, i);
    }
  }

  if (tree->root->is_leaf)
    return delete_record_from_node(tree, tree->root, r);
  else
    return delete_record_from_node(tree, node->children[i], r);
}


/*
 * ===========================================
 *  API functions implementations start here!
 * ===========================================
 */

jnx_btree* jnx_btree_create(jnx_int32 order, compare_keys callback) {
  if ( order <= 1 ) {
    return NULL;
  }

  jnx_btree *tree = calloc(1, sizeof(jnx_btree));

  tree->order = order;
  tree->compare_function = callback;
  tree->root = new_node(tree->order, 1);
  tree->internal_lock = jnx_thread_mutex_create();
  return tree;
}

void jnx_btree_add(jnx_btree *tree, void *key, void *value) {
  JNXCHECK(tree);
  JNXCHECK(key);
  JNXCHECK(value);
  if ( tree == NULL) {
    return;
  }

  record *r = malloc(sizeof(record));
  r->key = key;
  r->value = value;

  if ( is_node_full(tree, tree->root) ) {
    jnx_btree_node *temp = tree->root;
    tree->root = new_node(tree->order, 0);
    tree->root->children[0] = temp;

    split_child_at_index(tree, tree->root, 0);
  }

  insert_into_tree_at_node(tree, tree->root, r);
}
void jnx_btree_add_ts(jnx_btree *tree, void *key, void *value) {
  JNXCHECK(tree);
  JNXCHECK(key);
  JNXCHECK(value);
  if(!tree) { return ; }
  jnx_thread_lock(tree->internal_lock);	
  jnx_btree_add(tree,key,value);
  jnx_thread_unlock(tree->internal_lock);	
}
void *jnx_btree_lookup(jnx_btree *tree, void *key) {
  JNXCHECK(tree);
  JNXCHECK(key);
  if ( tree == NULL ) {
    return NULL;
  }
  return find_value_for_key_in_node(tree, tree->root, key);
}
void *jnx_btree_lookup_ts(jnx_btree *tree, void *key) {
  JNXCHECK(tree);
  JNXCHECK(key);
  if(!tree) { return NULL; }
  jnx_thread_lock(tree->internal_lock);	
  void *ret = jnx_btree_lookup(tree,key);
  jnx_thread_unlock(tree->internal_lock);	
  return ret;
}
void jnx_btree_remove(jnx_btree *tree, void *key_in, void** key_out, void **val_out ) {
  record *temp = NULL;
  if ( tree == NULL || tree->root->count == 0 ) {
    return;
  }
  JNXCHECK(tree);
  JNXCHECK(key_in);

  record *r = malloc(sizeof(record));
  r->key = key_in;
  r->value = key_in;

  temp = delete_record_from_node(tree, tree->root, r);
  if (temp != NULL) {
    if (key_out != NULL)
      *key_out = temp->key;
    if (val_out != NULL)
      *val_out = temp->value;
  }
  free(temp);
  free(r);
}
void jnx_btree_remove_ts(jnx_btree *tree, void *key_in, void** key_out, void **val_out ) {
  if(!tree) { return ; }
  JNXCHECK(tree);
  JNXCHECK(key_in);
  jnx_thread_lock(tree->internal_lock);
  jnx_btree_remove(tree,key_in,key_out,val_out);
  jnx_thread_unlock(tree->internal_lock);
}
void jnx_btree_destroy(jnx_btree* tree) {
  JNXCHECK(tree);
  if ( tree == NULL ) {
    return;
  }
  delete_node_and_subtrees(tree->root);

  jnx_thread_mutex_destroy(&tree->internal_lock);
  free(tree);
}
static void append_keys_from_node(jnx_btree_node *node, jnx_list *keys) {
  jnx_int32 i;
  for (i = 0; i < node->count; i++ )
    jnx_list_add(keys, node->records[i]->key);
}

static void collect_keys_from_node(jnx_btree_node *node, jnx_list *keys) {
  if (node == NULL)
    return;

  if ( node->is_leaf ) {
    append_keys_from_node(node, keys);
    return;
  }

  jnx_int32 i;
  for (i = 0; i <= node->count; i++ ) {
    collect_keys_from_node(node->children[i], keys);
    if ( i < node->count )
      jnx_list_add(keys, node->records[i]->key);
  }

}

void jnx_btree_keys(jnx_btree *tree, jnx_list *keys) {
  JNXCHECK(tree);
  collect_keys_from_node(tree->root, keys);
}
void jnx_btree_keys_ts(jnx_btree *tree, jnx_list *keys) {
  JNXCHECK(tree);
  if(!tree) { return ; }
  jnx_thread_lock(tree->internal_lock);
  jnx_btree_keys(tree,keys);
  jnx_thread_unlock(tree->internal_lock);
}
