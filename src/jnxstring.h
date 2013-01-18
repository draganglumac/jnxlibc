#ifndef __JNXSTRING_H__
#define __JNXSTRING_H__
///Returns a new malloced string with the result of conatenation of both strings
//WARNING:Does not free the original strings inputted!
char* jnx_string_joincreate(char* destination, char* buf);
///Concatenates the second string to the first, pass &ref for the destination to be altered
void jnx_string_join(char** destination, char* buf);
#endif
