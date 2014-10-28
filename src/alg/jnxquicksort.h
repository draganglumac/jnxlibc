/*
 * =====================================================================================
 *
 *       Filename:  quicksort.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  23/10/2014 14:28:35
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dragan Glumac (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __JNXQUICKSORT_H__
#define __JNXQUICKSORT_H__

typedef int(*compare_function)(void *, void *);

void jnx_quicksort(void **array, int size, compare_function cf);

#endif // __JNXQUICKSORT_H__
