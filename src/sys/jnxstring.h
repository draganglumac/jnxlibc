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
 * @brief Function that joins destination and buf and returns the new string.
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
 * @brief Function that appends buf to destination without recreating any strings.
 * @param destination this requires a pointer to the memory for the destination string
 * @param buf takes a buffer and appends it to the destination string
 */
void jnx_string_join(char** destination, char* buf);

/**
 * @fn jnx_string_itos(int input)
 * @brief Function that convets passed in integer to its string representation.
 * @param input requires an int input for conversion
 *
 * @return returns a string on the stack which is the converted input. NULL is returned on failure
 */
char* jnx_string_itos(int input);
#endif
