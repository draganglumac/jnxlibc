#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char* file_read(char* path)
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
	fread(from_file_str, 1, size, fp);
	fclose(fp);

	return from_file_str;
}
void mystring_join(char** destination, char* buf)
{//THERE ARE PROBLEMS WITH THIS!!!:
	int orig_len;
	int buf_len = strlen(buf);
	if (*destination == NULL) {
		orig_len = 0;
	} else {
		orig_len = strlen(*destination);
		printf("original length %d\n", orig_len); 
	}

	int newsize = orig_len + buf_len + 1;
	printf("Buffer len %d\n",newsize);

	*destination  = (char*) realloc(*destination,sizeof(newsize));

	strncpy(*destination + orig_len, buf, buf_len + 1);
}
int main(int argc, char** argv)
{
	char *out = file_read("Makefile");	
	printf("Raw makefile %s\n",out);
	mystring_join(&out,"!");
	printf("OUTPUT OF JOIN -> %s\n",out);


	return 0;
}
