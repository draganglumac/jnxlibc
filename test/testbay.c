#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxstring.h"


void join(char **destination, char *buf)
{
	int orig_len;
	int buf_len = strlen(buf);
	if (*destination == NULL){
		orig_len = 0;
	}
	else
	{
		orig_len = strlen(*destination);
	}
	*destination  = (char *) realloc(  *destination, (sizeof(orig_len + buf_len + 1)));
	if ( orig_len != 0 ) 
	{
		strncpy(*destination + orig_len, buf, buf_len + 1);
	} else {
		strncpy(*destination, buf, buf_len + 1);
	}
}

int main (int argc, char** argv)
{
	char *foo = malloc(strlen("foo")*sizeof(char));
	strcpy(foo,"test");
	char *bar = "barbarbar";
	
	jnx_string_join(foo,bar);


	printf("%s\n",foo);

    return 0;
}
