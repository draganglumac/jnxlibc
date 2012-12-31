#include "jnxstring.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
char* jnx_string_conc(char *destination,char *append)
{
	int origin_len = strlen(destination);
	int append_len = strlen(append);
	char *str = malloc(sizeof(origin_len + append_len));
	strcpy(str,destination);
	strcat(str,append);
	return str;
}