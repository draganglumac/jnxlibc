#include "list.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct node{
	void *_data;
	struct node *next_node;
};
typedef struct list{
	struct node *head;
};

struct list* list_make(void )
{
	struct list *temp = malloc(sizeof(struct list));
	
	return temp;
}
void list_add(struct list* A, void* _datain)
{
	struct node *temp = malloc(sizeof(struct node));
	temp->_data = _datain;
	temp->next_node = A->head;
	A->head = temp;
}
void* list_remove(struct list* A)
{
	if(A->head == NULL) return NULL;
	if(A->head->next_node == NULL)
		return A->head->_data;
	
	struct node *head_origin = A->head;
	
	while(A->head)
	{
		struct node *next = A->head->next_node;
		
		if(next->next_node == NULL)
		{
			void *temp = next->_data;
			A->head->next_node = NULL;  //Note that this doesn't delete the node, just the pointer to it. we return the node.
			A->head = head_origin;
			return temp;
		}
		A->head = next;
	}
	A->head = head_origin;
	return NULL;
}
void list_delete(struct list* A)
{
	if(!A->head) return;
	
	struct node *current = A->head;
	struct node *next = A->head->next_node;
	
	while(!next)
	{
		free(current);
		
		current = next;
		
		next = next->next_node;
	}
	
	free(current);
	
	A->head = NULL;
}

