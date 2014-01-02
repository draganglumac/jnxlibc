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
#include "jnxlist.h"
#define MAX_TRACE 40
#ifdef __JNX_DEBUG_DISABLE__
#define __JNX_DEBUG_MEMORY_UNMANAGED__
#endif
typedef enum { FREE, ALLOC }jnx_debug_memtrace_state;
typedef struct{
	void *ptr;
	size_t size;
	char *file;
	char *function;
	int line;
	jnx_debug_memtrace_state state;
	
}jnx_debug_memtrace_item;
/**
 *@fn jnx_debug_stacktrace
 *@brief Provides functionality to write stacktrace to a file on segmentation fault
 *@param path is the path to the save file, if this file is not valid it will print
 *@param counter tells how many signals used
 *@param signals is an array of signals to listen for e.g. [SIGKILL,SIGSEGV]
 */
void jnx_debug_stacktrace(char *path, int counter,int signals[]);

/** 
 *@fn jnx_debug_memtrace
 *@brief Prints the current memtrace to file
 *@param path is the path to the save file, if this file is not valid it will print
 */
void jnx_debug_memtrace(char *path);
/**
 *@fn jnx_debug_memtrace_clear_memory
 *@brief frees all memory and resets tracking stack
 *@warning this is obviously quite dangerous in real programs
 */
size_t jnx_debug_memtrace_clear_memory();

/**
 *@fn jnx_debug_memtrace_get_byte_alloc
 *@brief more of a debugging function this allows the user to get total bytes alloc
 *@return returns size_t of total number of bytes
 */
size_t jnx_debug_memtrace_get_byte_alloc();
/**
 *@fn jnx_debug_memtrace_get_total_number_alloc
 *@brief more of a debugging function this allows the user to get total allocs
 *@return returns size_t of total number of allocs
 */
size_t jnx_debug_memtrace_get_total_number_alloc();
/**
 *@fn jnx_debug_memtrace_get_current_number_alloc
 *@brief more of a debugging function this allows the user to get current allocs
 *@return returns size_t of current number of allocs
 */
size_t jnx_debug_memtrace_get_current_number_alloc();
/**
 *@fn jnx_debug_memtrace_get_list
 *@brief gets the current stack of memory operations
 *@param return jnx_stack pointer
 */
jnx_list *jnx_debug_memtrace_get_list();
/**
 *@fn jnx_debug_malloc
 *@brief overrides normal system function with a debugging version for memory leak testing
 *@param size is the size of bytes to malloc
 */
void* jnx_debug_malloc(size_t size,char *file,const char *function,int line);
/**
 *@fn jnx_debug_realloc
 *@brief overrides normal system function with a debugging version for memory leak testing
 *@param ptr is a pointer to the memory to resize
 *@param size is the size of new realloc
 *@param returns a pointer to memory if the block was too large for original location
 */
void* jnx_debug_realloc(void *ptr,size_t size,char *file,const char *function,int line);
/**
 *@fn jnx_debug_calloc
 *@brief overrides normal system function with a debugging version for memory leak testing
 *@param num is the length of memory blocks
 *@param size is the size of memory blocks
 *@return pointer to new memory
 */
void* jnx_debug_calloc(size_t num,size_t size,char *file,const char *function,int line);
/**
 *@fn jnx_debug_free
 *@brief overrides normal system function with a debugging version for memory leak testing
 *@param ptr to memory for deletion
 */
void jnx_debug_free(void *ptr);


#define JNX_DEBUG_MALLOC(X)\
	jnx_debug_malloc(X,__FILE__,__FUNCTION__,__LINE__);
#define JNX_DEBUG_CALLOC(X,Y) \
	jnx_debug_calloc(X,Y,__FILE__,__FUNCTION__,__LINE__);
#define JNX_DEBUG_REALLOC(X,Y)\
	jnx_debug_realloc(X,Y,__FILE__,__FUNCTION__,__LINE__);
#define JNX_DEBUG_FREE(X)\
	jnx_debug_free(X);
#endif




