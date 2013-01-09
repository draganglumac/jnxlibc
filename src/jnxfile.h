#ifndef __JNXFILE_H__
#define __JNXFILE_H__
typedef struct jnx_file_kvp_node
{
	char *key;
	char *value;
	struct jnx_file_kvp_node *next;
};
///Returns a callocd string with the file data, you will assume responsibility for freeing
char* jnx_file_read (char* path);
///Returns a linkedlist of nodes that contain Key-value pairs from file
struct jnx_file_kvp_node* jnx_file_read_keyvaluepairs(char *path);
///Writes a string directly to a file
int jnx_file_write (char* path, char* data);
///Warning use with caution, will recursively delete everything within program permission
int jnx_file_recursive_delete(char* path);
#endif
