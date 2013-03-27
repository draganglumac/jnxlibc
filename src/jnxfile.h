/** @file jnxfile.h
 *  @brief File manipulation, read/write, kvp plucking and recursive deletion
 *
 *  #include <jnxc_headers/jnxfile.h>
 */
#ifndef __JNXFILE_H__
#define __JNXFILE_H__
/**
 *  @brief an internal representation of the kvp node populated when used in key value pair retrieval from a file
 */
typedef struct jnx_file_kvp_node {
    char* key;
    char* value;
    struct jnx_file_kvp_node* next;
}jnx_file_kvp_node;

/**
 * @fn jnx_file_read(char* path)
 *
 * @param path path to the target file for reading
 *
 * @return a malloced char* of the file content
 *
 * @warning it is the users responsibility to manage the memory of the char*
 */
char* jnx_file_read(char* path);

/**
 * @fn jnx_file_read_keyvaluepairs(char *path, char* delimiter)
 *
 * @param path path to the target file for reading
 * @param delimiter the token to break the key value pairs on e.g. '='
 *
 * @return returns a linkedlist inhabited by jnx_file_kvp_nodes
 *
 * @warning user is responsible for linked list deletion and its node/data
 */
jnx_file_kvp_node* jnx_file_read_keyvaluepairs(char* path, char* delimiter);

/**
 * @fn jnx_file_write(char *path, char* data)
 *
 * @param path path to the target file for writing
 * @param data pointer to the char* for writing to the file
 *
 * @return success on 0
 */
int jnx_file_write(char* path, char* data);

/**
 * @fn jnx_file_recursive_delete(char* path)
 *
 * @param path the provided path will have all its sub directories and files deleted
 *
 * @return success on 0
 *
 * @warning extremely powerful and can make unrepairable changes to your file system
 */
int jnx_file_recursive_delete(char* path);
#endif
