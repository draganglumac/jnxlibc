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
#include "jnx_string.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "jnx_check.h"
jnx_char* jnx_string_joincreate(jnx_char* destination, jnx_char* append) {
  JNXCHECK(destination);
  JNXCHECK(append);
  jnx_int32 origin_len = strlen(destination);
  jnx_int32 append_len = strlen(append);
  jnx_char* str = malloc(sizeof(origin_len + append_len + 1));
  strcpy(str, destination);
  strcat(str, append);
  return str;
}
void jnx_string_join(jnx_char** destination, jnx_char* buf) {
  jnx_int32 orig_len;
  jnx_int32 buf_len = strlen(buf);
  if (*destination == NULL) {
    orig_len = 0;
  } else {
    orig_len = strlen(*destination);
  }
  jnx_int32 newsize = orig_len + buf_len + 1;
  *destination  = (jnx_char*) realloc(*destination,newsize);
  if (orig_len != 0) {
    strncpy(*destination + orig_len, buf, buf_len + 1);
  } else {
    strncpy(*destination, buf, buf_len + 1);
  }
}
jnx_char* jnx_string_itos(jnx_int32 input) {
  jnx_char *str = malloc(sizeof(jnx_int32));
  if(sprintf(str,"%d",input) > 0) {
    return str;
  } else {
    return NULL;
  }
}
