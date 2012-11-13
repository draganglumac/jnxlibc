#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/list.h"

int main(int argc, char **argv) {

	struct list* l = list_make();
	
	list_add(l,"hello");
	list_add(l,"hello");
	list_add(l,"up");
	list_add(l,"down");
	list_add(l,"bye");

	
	struct list *n;
	memcpy(n,l,sizeof(l));
	
	while(n->head)
	{
		printf("%s",(char*)n->head->_data);
		n->head = n->head->next_node;
	}
	
	
    return 0;
}
