/*
 * =====================================================================================
 *
 *       Filename:  jnxdebug.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/27/13 18:58:38
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __JNX_DEBUG_H__
#define __JNX_DEBUG_H__
#include <signal.h>
#define MAX_TRACE 40
/**
 *@fn jnx_debug_stacktrace
 *@brief Provides functionality to write stacktrace to a file on segmentation fault
 *@param path is the path to the save file, if this file is not valid it will print
 *@param counter tells how many signals used
 *@param signals is an array of signals to listen for e.g. [SIGKILL,SIGSEGV]
 */
void jnx_debug_stacktrace(char *path, int counter,int signals[]);
#endif
