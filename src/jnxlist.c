#include "jnxlist.h"
#include <stdlib.h>
#include <stdio.h>

jnx_list* jnx_list_make (void) {
    jnx_list* temp = malloc (sizeof (jnx_list));

    return temp;
}
void jnx_list_add (jnx_list* A, void* _datain)
{
    jnx_node* temp = malloc (sizeof (jnx_node));
    temp->_data = _datain;
    temp->next_node = A->head;
    A->head = temp;
}
void* jnx_list_remove (jnx_list* A)
{
    if (A->head == NULL) {
        return NULL;
    }
    if (A->head->next_node == NULL) {
        return A->head->_data;
    }

    jnx_node* head_origin = A->head;

    while (A->head) {
        jnx_node* next = A->head->next_node;

        if (next->next_node == NULL) {
            void* temp = next->_data;
            A->head->next_node = NULL;  //Note that this doesn't delete the node, just the pointer to it. we return the node.
            A->head = head_origin;
            return temp;
        }
        A->head = next;
    }
    A->head = head_origin;
    return NULL;
}
void jnx_list_delete (jnx_list* A)
{
    if (!A->head) {
        return;
    }

    jnx_node* current = A->head;
    jnx_node* next = A->head->next_node;

    while (!next) {
        free (current);

        current = next;

        next = next->next_node;
    }

    free (current);

    A->head = NULL;

}
void* jnx_list_next (jnx_list* A)
{
    if (A->head == NULL) {
        return NULL;
    }

    void* data = A->head->_data;
    A->head = A->head->next_node;
    return data;
}

