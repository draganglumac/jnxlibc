#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxfile.h"


int main(int argc, char **argv)
{
// 	printf("starting\n");
// 	char *res = jnx_file_tostr("Makefile");
// 	printf("%s",res);

	char *text = "TEST WRITING TO FILE\n";
	jnx_file_fromstr("test.txt",text);
	
	printf("From our file -> %s\n",jnx_file_tostr("test.txt"));
	return 0;
}
