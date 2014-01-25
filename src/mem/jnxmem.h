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

/**
 *@fn jnx_mem_get_total_number_allocations()
 *@return size_t of number of allocations in program 
 */
size_t jnx_mem_get_total_number_allocations();
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

#if defined(DEBUG) || defined(Debug)
#define JNX_MEM_MALLOC(X)\
	jnx_mem_malloc(X,__FILE__,__FUNCTION__,__LINE__);
#define JNX_MEM_CALLOC(X,Y) \
	jnx_mem_calloc(X,Y,__FILE__,__FUNCTION__,__LINE__);
#define JNX_MEM_REALLOC(X,Y)\
	jnx_mem_realloc(X,Y,__FILE__,__FUNCTION__,__LINE__);
#define JNX_MEM_FREE(X)\
	jnx_mem_free(X);
#else
#define JNX_MEM_MALLOC(X)\
	malloc(X);
#define JNX_MEM_CALLOC(X,Y) \
	calloc(X,Y);
#define JNX_MEM_REALLOC(X,Y)\
	realloc(X,Y);
#define JNX_MEM_FREE(X)\
	free(X);
#endif
#ifdef __cplusplus
	}
#endif
#endif
