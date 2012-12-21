#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxfile.h"


int main(int argc, char **argv)
{
	printf("starting\n");
	char *res = jnx_file_tostr("Makefile");
	printf("%s",res);
	return 0;
}
