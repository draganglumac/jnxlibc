#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "../src/list.h"

int main(int argc, char **argv) {

	struct list* l = list_make();
	
	list_add(l,"hello");
	list_add(l,"hello");
	list_add(l,"up");
	list_add(l,"down");
	list_add(l,"bye");

	list_delete(l);
	
	while(l->head)
	{
		printf("%s\n",(char*)l->head->_data);
		
		l->head = l->head->next_node;
	}
	
	
    return 0;
}
