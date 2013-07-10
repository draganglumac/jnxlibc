/** @file jnxfile.h
 *  @brief File manipulation, read/write, kvp plucking and recursive deletion
 *
 *  #include <jnxc_headers/jnxfile.h>
 */
#ifndef __JNXFILE_H__
#define __JNXFILE_H__
#include <stddef.h>
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
 * @param buffer that will be filled
 *
 * @return bytes_read
 *
 * @warning it is the users responsibility to manage the memory of the char*
 */
int jnx_file_readb(char* path, char **buffer);
/**
 * @fn jnx_file_read(char* path)
 *
 * @param path path to the target file for reading
 *
 * @param buffer that will be filled
 *
 * @return bytes_read
 *
 * @warning it is the users responsibility to manage the memory of the char*
 */
int jnx_file_read(char* path, char **buffer);

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
 * @param size_t data_size of data to be written
 * @return bytes written on success
 */
size_t jnx_file_write(char* path, char* data, size_t data_size);
/**
 * @fn jnx_file_writeb(char *path, char* data)
 *
 * @param path path to the target file for writing in byte mode
 * @param data pointer to the char* for writing to the file
 * @param size_t data_size to be written
 * @return bytes written on success
 */
size_t jnx_file_writeb(char* path, char* data,size_t data_size);

/**
 * @fn jnx_file_recursive_delete(char* path)
 *
 * @param path the provided path will have all its sub directories and files deleted
 * @param depth defines how deep to recursively delete in a folder structure
 * @return success on 0
 *
 * @warning extremely powerful and can make unrepairable changes to your file system
 */
int jnx_file_recursive_delete(char* path,int depth);
#endif
