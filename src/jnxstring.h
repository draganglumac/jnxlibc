#ifndef __JNXSTRING_H__
#define __JNXSTRING_H__
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_string_joincreate
 *  Description:  Creates a new string returned from the two inputs, does not free original
 * =====================================================================================
 */
char* jnx_string_joincreate(char* destination, char* buf);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_string_join
 *  Description:  Concats the strings together
 * =====================================================================================
 */
void jnx_string_join(char** destination, char* buf);
#endif
