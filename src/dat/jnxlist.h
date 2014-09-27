/** @file jnxlist.h
 *  @brief API for jnxlibc implementation of list
 *
 *  #include <jnxc_headers/jnxlist.h>
 */
#ifndef __JNXLIST_H__
#define __JNXLIST_H__
#include "jnxthread.h"
#ifdef __cplusplus
extern		"C" {
#endif
  /**
   * @brief Node represents a link in the list
   */
  typedef struct node {
    void *_data;
    struct node *prev_node;
    struct node *next_node;
  }		jnx_node;
  /**
   * @brief Represents the list data structure as seen by the user
   */
  typedef struct list {
    struct node *head;
    struct node *tail;
    int counter;
    jnx_thread_mutex *internal_lock;
  }		jnx_list;

  /** @fn jnx_list_create(void)
   *  @brief Returns newly created list
   *  @return jnx_list* to a newly created list of 0 length
   */
  jnx_list *jnx_list_create(void);

  /** @fn jnx_list_add(jnx_list *A, void *datain)
   *  @brief  Accepts a void pointer for input with list
   *  @param A is the target list
   *  @param datain is the data to insert
   */
  void jnx_list_add(jnx_list * A, void *datain);

  void jnx_list_add_ts(jnx_list * A, void *datain);
  /** @fn jnx_list_remove(jnx_list **A)
   *  @brief Removes the structure from list but returns the data for the user to handle
   *  @param A is a pointer to the pointer of the list to remove the last link of
   *  @return void* to data, NULL on failure or end of list
   */
  void *jnx_list_remove(jnx_list ** A);

  void *jnx_list_remove_ts(jnx_list ** A);

  /** @fn jnx_list_remove_front(jnx_list **A)
   *  @brief removes jnx_node structure from the list and returns data to the user
   *  @param A is a pointer to pointer of the list to remove the first link of
   *  @return void* to data, NULL on failure or end of list
   */
  void *jnx_list_remove_front(jnx_list ** A);

  void *jnx_list_remove_front_ts(jnx_list ** A);

  /** @fn jnx_list_size(jnx_list *A)
   *  @param A is the target list
   *  @return the number of elements in the list
   */
  size_t jnx_list_count(jnx_list * A);

  size_t jnx_list_count_ts(jnx_list * A);

  /** @fn jnx_list_contains(jnx_list *A, void *datain, int (*list_comparison)(void *a,void *b))
   *  @brief checks whether a list contains a specific value
   *  @param A is a pointer to the list 
   *  @param datain is the value to check the list for
   *  @param list_comparison is a functor callback for comparison, 1 is a positive find, 0 is a negative
   *  @return either 1 on success or 0 on failure
   */
  int jnx_list_contains(jnx_list *A, void *datain, int (*list_comparison)(void *a,void *b));

  int jnx_list_contains_ts(jnx_list *A, void *datain, int (*list_comparison)(void *a,void *b));
  /** @fn jnx_list_destroy(jnx_list* A)
   * @brief  Deletes the list node structure and list, not data inside
   * @param A is a pointer the pointer of list to destroy
   */
  void jnx_list_destroy(jnx_list ** A);

#ifdef __cplusplus
}
#endif
#endif
