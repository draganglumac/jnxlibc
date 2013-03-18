#ifndef __JNXFILE_H__
#define __JNXFILE_H__
typedef struct jnx_file_kvp_node {
    char* key;
    char* value;
    struct jnx_file_kvp_node* next;
}jnx_file_kvp_node;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_file_read
 *  Description:  Returns a calloced string that the user is responsible for freeing
 * =====================================================================================
 */
char* jnx_file_read(char* path);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_file_read_keyvaluepairs
 *  Description:  Returns a linkedlist from kvp found in the target file
 * =====================================================================================
 */
jnx_file_kvp_node* jnx_file_read_keyvaluepairs(char* path, char* delimiter);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_file_read_keyvaluepairs
 *  Description:  Writes a string directly into a file, no fuss
 * =====================================================================================
 */
int jnx_file_write(char* path, char* data);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_file_read_keyvaluepairs
 *  Description:  Recursively deletes a path, use with caution
 * =====================================================================================
 */
int jnx_file_recursive_delete(char* path);
#endif
