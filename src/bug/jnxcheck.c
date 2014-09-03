/*
 * =====================================================================================
 *
 *       Filename:  jnxcheck.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/16/14 08:17:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxcheck.h"
#include <execinfo.h>
#include <stdio.h>
void jnxcheck_backtrace() {
  void *array[MAXSTACKFRAMES];
  size_t size;
  char **strings;
  size_t i;
  size = backtrace (array, MAXSTACKFRAMES);
  strings = backtrace_symbols (array, size);
  JNX_LOG(DEFAULT_CONTEXT,"Obtained %zd stack frames.\n", size);
  for (i = 0; i < size; i++)
    JNX_LOG(DEFAULT_CONTEXT,"%s\n", strings[i]);
  free (strings);
}
