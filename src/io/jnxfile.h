/** @file jnxfile.h
 *  @brief File manipulation, read/write, kvp plucking and recursive deletion
 *
 *  #include <jnxc_headers/jnxfile.h>
 */
#ifndef __JNXFILE_H__
#define __JNXFILE_H__
#include <stddef.h>
#include "jnxhash.h"
#ifdef __cplusplus
	extern "C" {
#endif
/**
 * @brief An internal representation of the kvp node populated when 
 *			used in key value pair retrieval from a file
 */
typedef struct jnx_file_kvp_node {
    char* key;
    char* value;
    struct jnx_file_kvp_node* next;
}jnx_file_kvp_node;
/**
 * @fn jnx_file_read(char* path, char **buffer,char *flags)
 * @brief Function to read the contents of a text file into the buffer.
 *
 * @param path path to the target file for reading
 * @param buffer that will be filled
 * @param flags are the flags passed (r,rb...)
 * @return Number of bytes read
 *
 * @warning it is the users responsibility to manage the memory of the char*
 */
size_t jnx_file_read(char* path, char **buffer,char *flags);
/**
 * @fn jnx_hashmap *jnx_file_read_kvp(char *path, size_t max_buffer,char *flags)
 * @brief Function to read a file that contains key-value pairs into a linked list.
 *
 * @param path path to the target file for reading
 * @param delimiter the token to break the key value pairs on e.g. '='
 * @param max_buffer is maximum line size buffer
 * @return returns a linkedlist of jnx_file_kvp_nodes, i.e. key-value pairs.
 *
 * @warning user is responsible for linked list deletion and its node/data
 */
jnx_hashmap *jnx_file_read_kvp(char *path, size_t max_buffer, char *delimiter);

/**
 * @fn jnx_file_write(char *path, char* data, size_t data_size)
 * @brief Function to write the data buffer into the text file pointed to by path.
 *
 * @param path path to the target file for writing
 * @param data pointer to the char* for writing to the file
 * @param data_size size of data to be written
 * @param flags are the flags passed (w,r,a+ ...)
 * @return Number of bytes written on success
 */
size_t jnx_file_write(char* path, char* data, size_t data_size,char *flags);
/**
 * @fn jnx_file_recursive_delete(char* path, int depth)
 * @brief Function to recursively delete a directory pointed to by path.
 *
 * @param path the provided path will have all its sub directories and files deleted
 * @param depth defines how deep to recursively delete in a folder structure
 * @return success on 0
 *
 * @warning extremely powerful and can make unrepairable changes to your file system
 */
int jnx_file_recursive_delete(char* path, int depth);

/**
 * @fn jnx_file_mktempdir(char *template, char **path) 
 * @brief Function to create a temporary directory on the given path
 * @param template is the path to create the directory on
 * @param path is a pointer to the output buffer char pointer
 * @return success on 0
 */
int jnx_file_mktempdir(char *dirtemplate, char **path);
/**
 * @fn jnx_file_exists(char *file)
 * @brief checks if a file exists
 * @param filepath to check 
 * @returns 1 on file existance
 */
int jnx_file_exists(char *file);
#ifdef __cplusplus
	}
#endif
#endif
