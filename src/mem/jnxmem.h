/** @file jnxmem.h
 * @brief The API for jnxlibc's implementation of simple memory management for DEBUG mode
 *
 * #include <jnxc_headers/jnxmem.h>
 */


#ifndef __JNX_MEM_H__
#define __JNX_MEM_H__
#include "jnxfile.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#ifdef __cplusplus
	extern "C" {
#endif
typedef enum { FREE, ALLOC }jnx_mem_memtrace_state;
typedef struct{
	void *ptr;
	size_t size;
	char *file;
	char *function;
	int line;
	jnx_mem_memtrace_state state;
	
}jnx_mem_item;

/** 
 *@fn jnx_mem_memtrace
 *@brief Prints the current memtrace to file
 *@param path is the path to the save file, if this file is not valid it will print
 */
void jnx_mem_trace(char *path);
/**
 *@fn jnx_mem_memtrace_clear_memory
 *@brief frees all memory and resets tracking stack
 *@warning this is obviously quite dangerous in real programs
 */
size_t jnx_mem_clear();

/**
 *@fn jnx_mem_memtrace_get_byte_alloc
 *@brief more of a memging function this allows the user to get total bytes alloc
 *@return returns size_t of total number of bytes
 */
size_t jnx_mem_get_byte_alloc();
/**
 *@fn jnx_mem_memtrace_get_total_number_alloc
 *@brief more of a memging function this allows the user to get total allocs
 *@return returns size_t of total number of allocs
 */
size_t jnx_mem_get_total_number_alloc();
/**
 *@fn jnx_mem_memtrace_get_current_number_alloc
 *@brief more of a memging function this allows the user to get current allocs
 *@return returns size_t of current number of allocs
 */
size_t jnx_mem_get_current_number_alloc();
/**
 *@fn jnx_mem_malloc
 *@brief overrides normal system function with a memging version for memory leak testing
 *@param size is the size of bytes to malloc
 */
void* jnx_mem_malloc(size_t size,char *file,const char *function,int line);
/**
 *@fn jnx_mem_realloc
 *@brief overrides normal system function with a memging version for memory leak testing
 *@param ptr is a pointer to the memory to resize
 *@param size is the size of new realloc
 *@param returns a pointer to memory if the block was too large for original location
 */
void* jnx_mem_realloc(void *ptr,size_t size,char *file,const char *function,int line);
/**
 *@fn jnx_mem_calloc
 *@brief overrides normal system function with a memging version for memory leak testing
 *@param num is the length of memory blocks
 *@param size is the size of memory blocks
 *@return pointer to new memory
 */
void* jnx_mem_calloc(size_t num,size_t size,char *file,const char *function,int line);
/**
 *@fn jnx_mem_free
 *@brief overrides normal system function with a memging version for memory leak testing
 *@param ptr to memory for deletion
 */
void jnx_mem_free(void *ptr);


#define JNX_MEM_MALLOC(X)\
	jnx_mem_malloc(X,__FILE__,__FUNCTION__,__LINE__);
#define JNX_MEM_CALLOC(X,Y) \
	jnx_mem_calloc(X,Y,__FILE__,__FUNCTION__,__LINE__);
#define JNX_MEM_REALLOC(X,Y)\
	jnx_mem_realloc(X,Y,__FILE__,__FUNCTION__,__LINE__);
#define JNX_MEM_FREE(X)\
	jnx_mem_free(X);

#ifdef __cplusplus
	}
#endif
#endif
