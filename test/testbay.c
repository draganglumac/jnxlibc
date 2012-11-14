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

    return 0;
}
