#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxlist.h"

typedef struct node _node;

int main(int argc, char **argv) {

	struct list *l = jnx_list_make();
	
	jnx_list_add(l,"char");
	jnx_list_add(l,"hello");
	jnx_list_add(l,"is");
	
	
	_node *head = l->head;
	
	_node *out = head;
	
	while(head)
	{
		printf("%s\n",head->_data);
		head = head->next_node;
	}
	
	head = out;
	
	while(head)
	{
		printf("%s\n",head->_data);
		head = head->next_node;
	}
	
	jnx_list_delete(l);

    return 0;
}
