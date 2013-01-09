#include "jnxfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
char* jnx_file_read (char* path)
{
    FILE* fp;
    if ( (fp = fopen (path, "r")) == NULL) {
        perror ("file: ");
    }
    fseek (fp, 0, SEEK_END);
    long int size = ftell (fp);
    rewind (fp);
    char* from_file_str = calloc (size + 1, 1);
    fread (from_file_str, 1, size, fp);
    fclose (fp);
    return from_file_str;
}
struct jnx_file_kvp_node* jnx_file_read_keyvaluepairs(char* path)
{
	FILE *file = fopen(path,"r+");
	
	struct jnx_file_kvp_node *list = NULL, **nextp = &list;
	char buffer[1024];

	while (fgets(buffer, sizeof buffer, file) != NULL) 
	{
    struct jnx_file_kvp_node *node;
    node = malloc(sizeof(struct jnx_file_kvp_node) + strlen(buffer) + 1);
    node->key = strtok(strcpy((char*)(node+1), buffer), "=");
    node->value = strtok(NULL, "=");
    node->next = NULL;
    *nextp = node;
    nextp = &node->next;
	}
	return list;
}
int jnx_file_write (char* path, char* data)
{
    FILE* fp;
    if ( (fp = fopen (path, "w")) == NULL) {
        perror ("file: ");
        return 1;
    }
    int size = strlen (data);
    fwrite (data, size, 1, fp);
    fclose (fp);
    return 0;
}
int jnx_file_recursive_delete(char* path)
{
    DIR *dp;
    struct dirent *ep;

    char *dirname= malloc(sizeof((char) * path)+1);

    dirname[strlen(path)]=0;

    memcpy(dirname,path,strlen(path));

    char abs_filename[FILENAME_MAX];

    dp = opendir (dirname);
    if (dp != NULL)
    {
        while ((ep = readdir (dp))) {
            struct stat stFileInfo;

            snprintf(abs_filename, FILENAME_MAX, "%s/%s", dirname, ep->d_name);

            if (lstat(abs_filename, &stFileInfo) < 0)
                perror ( abs_filename );

            if(S_ISDIR(stFileInfo.st_mode)) {
                if(strcmp(ep->d_name, ".") &&
                        strcmp(ep->d_name, "..")) {
                    printf("%s directory\n",abs_filename);
                    jnx_file_recursive_delete(abs_filename);
                }
            } else {
                printf("%s file\n",abs_filename);
                remove(abs_filename);
            }
        }
        (void) closedir (dp);
    }
    else
	{
        perror ("Couldn't open the directory");
		return 1;
	}

    remove(dirname);
    return 0;
}
