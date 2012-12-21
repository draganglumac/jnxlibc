#include "jnxfile.h"
#include <stdio.h>
#include <stdlib.h>

char *jnx_file_tostr(char *path)
{
	FILE *fp;
	if((fp = fopen(path,"r")) == NULL)
	{
		perror("file: ");
	}
	fseek(fp,0,SEEK_END);
	long int size = ftell(fp);
	rewind(fp);
	char *from_file_str = calloc(size + 1, 1);
	fread(from_file_str,1,size,fp);
	return from_file_str;
}
