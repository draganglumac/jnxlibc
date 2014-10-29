/*
 * =====================================================================================
 *
 *       Filename:  jnxstring.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/10/14 22:14:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com),
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "jnxstring.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "jnxcheck.h"
char* jnx_string_joincreate(char* destination, char* append) {
  JNXCHECK(destination);
  JNXCHECK(append);
  int origin_len = strlen(destination);
  int append_len = strlen(append);
  char* str = malloc(sizeof(origin_len + append_len + 1));
  strcpy(str, destination);
  strcat(str, append);
  return str;
}
void jnx_string_join(char** destination, char* buf) {
  int orig_len;
  int buf_len = strlen(buf);
  if (*destination == NULL) {
    orig_len = 0;
  } else {
    orig_len = strlen(*destination);
  }
  int newsize = orig_len + buf_len + 1;
  *destination  = (char*) realloc(*destination,newsize);
  if (orig_len != 0) {
    strncpy(*destination + orig_len, buf, buf_len + 1);
  } else {
    strncpy(*destination, buf, buf_len + 1);
  }
}
char* jnx_string_itos(int input) {
  char *str = malloc(sizeof(int));
  if(sprintf(str,"%d",input) > 0) {
    return str;
  } else {
    return NULL;
  }
}
