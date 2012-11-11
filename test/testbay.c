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

	
	char *ret1 = (char*)list_remove(l);
	printf("%s",ret1);
	
	char *ret2 = (char*)list_remove(l);
	printf("%s",ret2);
	
	char *ret3 = (char*)list_remove(l);
	printf("%s",ret3);
	
	char *ret4 = (char*)list_remove(l);
	printf("%s",ret4);
	
	char *ret5 = (char*)list_remove(l);
	printf("%s",ret5);
	
	
    return 0;
}
