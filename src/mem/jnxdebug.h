/** @file jnxdebug.h
 * @brief The API for jnxlibc's debugging tools
 *
 * #include <jnxc_headers/jnxdebug.h>
 */

#ifndef __JNX_DEBUG_H__
#define __JNX_DEBUG_H__
#include <signal.h>
#include "jnxlist.h"
#ifdef __cspluplus
	extern "C" {
#endif
#define MAX_TRACE 40
/**
 *@fn jnx_debug_stacktrace
 *@brief Provides functionality to write stacktrace to a file on segmentation fault
 *@param path is the path to the save file, if this file is not valid it will print
 *@param counter tells how many signals used
 *@param signals is an array of signals to listen for e.g. [SIGKILL,SIGSEGV]
 */
void jnx_debug_stacktrace(char *path, int counter,int signals[]);
#ifdef __cspluplus
	}
#endif
#endif
