/*
 * =====================================================================================
 *
 *       Filename:  jnxdebug.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/27/13 18:59:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxdebug.h"
#include "jnxfile.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <execinfo.h>
#include <string.h>
typedef void (*jnx_debug_internal_handler)(int);
static char *write_path = NULL;
void jnx_debug_stacktrace_cb(int s)
{
	void *array[MAX_TRACE];
	size_t size;
	char **str;
	size_t i;
	size = backtrace(array,MAX_TRACE);
	str=backtrace_symbols(array,size);
	for(i = 0; i < size; ++i)
	{
		printf("[%zu]%s\n",i,str[i]);
		int len = strlen(str[i]);
		str[i][len] = '\n';
		jnx_file_write(write_path,str[i],strlen(str[i]),"a");
	}
	free(str);
	exit(1);
}
void jnx_debug_stacktrace(char *path, int counter,int signals[])
{
	write_path = path;
	assert(write_path);	
	jnx_debug_internal_handler c = jnx_debug_stacktrace_cb;
	int i;
	for(i=0;i<counter;++i)
	{
		signal(signals[i],c);
	}
}
