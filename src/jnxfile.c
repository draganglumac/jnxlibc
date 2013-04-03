#include "jnxfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#define __USE_XOPEN_EXTENDED
#include <ftw.h>
#include <libgen.h>
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
    fread(from_file_str, 1, size, fp);
    fclose(fp);

    return from_file_str;
}

jnx_file_kvp_node* jnx_file_read_keyvaluepairs(char* path, char* delimiter) {
    FILE* file;

    if((file = fopen(path, "r+")) == NULL)
    {
        printf("Could not open file for KVP matching\n");
        return NULL;
    }

    jnx_file_kvp_node* list = NULL, ** nextp = &list;
    char buffer[1024];

    while (fgets(buffer, sizeof buffer, file) != NULL) {
        jnx_file_kvp_node* node;
        node = malloc(sizeof(jnx_file_kvp_node) + strlen(buffer) + 1);
        node->key = strtok(strcpy((char*)(node + 1), buffer), delimiter);
        node->value = strtok(NULL, delimiter);
        if(node->value[strlen(node->value)-1] == '\n'){
            node->value[strlen(node->value) -1] = 0;
        }
        node->next = NULL;
        *nextp = node;
        nextp = &node->next;
    }
    return list;
}
int jnx_file_write(char* path, char* data)
{
    FILE* fp;
    if ((fp = fopen(path, "w")) == NULL) {
        perror("file: ");
        return 1;
    }
    int size = strlen(data);
    fwrite(data, size, 1, fp);
    fclose(fp);
    return 0;
}
int jnx_file_recursive_delete(char* path)
{
	DIR *dir = opendir(path);
	struct dirent *d;
	if(dir == NULL) { return 1; }
	while (( d = readdir(dir)) != NULL) 
	{
		if(strcmp(".",d->d_name) == 0)
			continue;
		if(strcmp("..",d->d_name) == 0)
			continue;
		
		printf("%s\n",d->d_name);
		struct stat _sb;
		stat(d->d_name,&_sb);
		if(S_ISREG(_sb.st_mode))
		{
			remove(d->d_name);
		}
		else
		{
			printf("%s is a directory\n",d->d_name);
			jnx_file_recursive_delete(d->d_name);
			remove(d->d_name);
		}
	}
	closedir(dir);

	return 0;
}
