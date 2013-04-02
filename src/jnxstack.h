#ifndef __JNXSTACK_H__
#define __JNXSTACK_H__
/** @file jnxstack.h
 *  @brief API for jnxlibc implementation of stack data structure
 *
 *  #include <jnxc_headers/jnxstack.h>
 */

/**
 * @brief Node represents an element of the stack 
 */
typedef struct node {
    void* _data;
    struct node* next_node;
} jnx_node;
/**
 * @brief Represents the stack data structure as seen by the user
 */
typedef struct stack {
    struct node* top;
    int count;
} jnx_stack;

/** @fn jnx_stack_init(void)
 *  @brief Returns the newly created stack.
 *  @return jnx_stack* to a newly created stack of 0 length.
 */
jnx_stack* jnx_stack_init(void);

/** @fn jnx_stack_is_empty(jnx_stack *A)
 *  @brief Returns 1 (true) if the stack is empty, and 0 otherwise.
 *  @param A is the target stack.
 */

int jnx_stack_is_empty(jnx_stack* A);

/** @fn jnx_stack_is_empty(jnx_stack *A)
 *  @brief  Accepts a void pointer to input data to push onto the stack
 *  @param A is the target stack
 *  @param _datain is the data to insert
 */
void jnx_stack_push(jnx_stack* A, void* _datain);

/** @fn jnx_stack_pop(jnx_stack *A) 
 *  @brief Pops the top element off the stack and returns it.
 *  @param A is a pointer to the target stack.
 *  @return The data element that was just popped off the stack or NULL if the
 *  stack is empty.
 */
void* jnx_stack_pop(jnx_stack* A);

/** @fn jnx_stack_delete(jnx_stack* A) 
 * @brief  Deletes the stack's internal representation, but it doesn't try to
 * delete the _datain void pointers passed in. If you call jnx_stack_is_empty
 * after the delete, ti will return 1 (true).
 * @param A is a pointer to the stack to delete.
 */
void jnx_stack_delete(jnx_stack* A);
#endif

