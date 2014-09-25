/** @file jnxstack.h
 *  @brief API for jnxlibc implementation of stack data structure
 *
 *  #include <jnxc_headers/jnxstack.h>
 */
#ifndef __JNXSTACK_H__
#define __JNXSTACK_H__
#include "jnxthread.h"
#ifdef __cplusplus
extern "C" {
#endif
  /**
   * @brief Node represents an element of the stack 
   */
  typedef struct jnx_snode{
    void* _data;
    struct jnx_snode* next_node;
  } jnx_snode;
  /**
   * @brief Represents the stack data structure as seen by the user
   */
  typedef struct {
    struct jnx_snode* top;
    int count;
    jnx_thread_mutex *internal_lock;
  } jnx_stack;

  /** @fn jnx_stack_create(void)
   *  @brief Returns the newly created stack.
   *  @return jnx_stack* to a newly created stack of 0 length.
   */
  jnx_stack* jnx_stack_create(void);

  /** @fn jnx_stack_is_empty(jnx_stack *A)
   *  @brief Returns 1 (true) if the stack is empty, and 0 otherwise.
   *  @param A is the target stack.
   */
  int jnx_stack_is_empty(jnx_stack* A);

  int jnx_stack_is_empty_ts(jnx_stack* A);
  /** @fn jnx_stack_push(jnx_stack *A, void* _datain)
   *  @brief  Accepts a void pointer to input data to push onto the stack
   *  @param A is the target stack
   *  @param _datain is the data to insert
   */
  void jnx_stack_push(jnx_stack* A, void* _datain);

  void jnx_stack_push_ts(jnx_stack* A, void* _datain);
  /** @fn jnx_stack_pop(jnx_stack *A) 
   *  @brief Pops the top element off the stack and returns it.
   *  @param A is a pointer to the target stack.
   *  @return The data element that was just popped off the stack or NULL if the
   *  stack is empty.
   */
  void* jnx_stack_pop(jnx_stack* A);

  void* jnx_stack_pop_ts(jnx_stack* A);
  
  int jnx_stack_contains(jnx_stack *A, void *datain, int (*stack_comparison)(void *a, void *b));
  
  int jnx_stack_contains_ts(jnx_stack *A, void *datain, int (*stack_comparison)(void *a, void *b));
  
  /** @fn jnx_stack_destroy(jnx_stack* A) 
   * @brief  Deletes the stack's internal representation, but it doesn't try to
   * destroy the _datain void pointers passed in. If you call jnx_stack_is_empty
   * after the destroy, ti will return 1 (true).
   * @param A is a pointer to the stack to destroy.
   */
  void jnx_stack_destroy(jnx_stack** A);
#ifdef __cplusplus
}
#endif
#endif

