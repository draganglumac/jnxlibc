#ifndef __JNXLIST_H__
#define __JNXLIST_H__
typedef struct node {
    void* _data;
    struct node* next_node;
} jnx_node;
typedef struct list {
    struct node* head;
    int counter;
} jnx_list;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_list_init
 *  Description:  Returns newly created list
 * =====================================================================================
 */
jnx_list* jnx_list_init(void);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_list_init
 *  Description:  Accepts a void pointer for input with list
 * =====================================================================================
 */
void jnx_list_add(jnx_list* A, void* _datain);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_list_remove
 *  Description:  Removes the structure from list but returns the data for the user to handle
 * =====================================================================================
 */
void* jnx_list_remove(jnx_list* A);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_list_delete
 *  Description:  Deletes the list node structure and list, not data inside
 * =====================================================================================
 */
void jnx_list_delete(jnx_list* A);
#endif

