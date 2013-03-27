/**
 * @file jnxstring.h
 * @brief manipulate char* by joining
 *
 * #include <jnxc_headers/jnxstring.h> 
 */
#ifndef __JNXSTRING_H__
#define __JNXSTRING_H__
/**
 * @fn jnx_string_joincreate(char *destination, char *buf)
 *
 * @param destination the destination string for joining, must be malloced
 * @param buf the input buffer to be joined to destination
 *
 * @return returns a new char* string
 *
 * @warning user is responsible for memory management of new returned string and originals
 */
char* jnx_string_joincreate(char* destination, char* buf);

/**
 * @fn jnx_string_join(char **destination, char* buf)
 *
 * @param destination this requires a pointer to the memory for the destination string
 * @param buf takes a buffer and appends it to the destination string
 */
void jnx_string_join(char** destination, char* buf);
#endif
