#include "jnxfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *jnx_file_read(char *path)
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
	fclose(fp);
	return from_file_str;
}
int jnx_file_write(char *path, char *data)
{
	FILE *fp;
	if((fp = fopen(path,"w")) == NULL)
	{
		perror("file: ");
	 	return 1;
	}
	int size = strlen(data);
	fwrite(data,size,1,fp);
	fclose(fp);
	return 0;
}
