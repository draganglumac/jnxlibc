#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/list.h"

int main(int argc, char **argv) {

	struct list* l = list_make();
	
	list_add(l,"hello herp dp aoimaoi oimab \n aoimvao \n");
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
	
	list_delete(l);
	printf("List pointer deleted i think, and should be just garbage");
    return 0;
}
