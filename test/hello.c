#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include "../src/jnxhash.h"
//#include "../src/jnxterm.h"
//#include "../src/jnxnetwork.h"
//#include "../src/jnxfile.h"
 
char* jnx_file_read(char* path)
{
        FILE* fp;
        if ((fp = fopen(path, "r")) == NULL) {
                perror("file: ");
                return NULL;
        }
        if(fseek(fp, 0, SEEK_END) != 0)
        {
                perror("file: ");
                exit(1);
        }
        long int size = ftell(fp);
        rewind(fp);
        char* from_file_str = calloc(size + 1, sizeof(char));
        printf("Callocing size of %d\n",size + 1);
        fread(from_file_str, sizeof(char), size, fp);
        fclose(fp);
 
        return from_file_str;
}
void string_join(char** destination, char* buf)
{//THERE ARE PROBLEMS WITH THIS!!!:
        int orig_len;
        int buf_len = strlen(buf);
        if (*destination == NULL) {
                orig_len = 0;
        } else {
                orig_len = strlen(*destination);
        }
 
        int newsize = orig_len + buf_len + 1;
        printf("Buffer len %d\n",newsize);

	char *temp = (char*) realloc(*destination, newsize);
 
	strncpy(temp + orig_len, buf, buf_len + 1);

	*destination = temp;

       // strncpy(*temp + orig_len, buf, buf_len + 1);
}
int main(int argc, char** argv)
{
        char *out = jnx_file_read("Makefile.test");  
 
        char *testo = strdup(out);
 
        printf("Raw makefile %s\n",out);
 
        string_join(&out,"! Fred's your auntie.");
 
        printf("OUTPUT OF JOIN -> %s\n",out);
 
 
        return 0;
}
