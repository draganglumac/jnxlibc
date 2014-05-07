/*
 * =====================================================================================
 *
 *       Filename:  jnxquicksort.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/07/14 07:23:58
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxquicksort.h"

int jnxquicksort( int *array, int len) {
	if(len < 2) {
		return 1;
	}
	int pivot = array[len /2];
	int *l = array;
	int *r = array + len - 1;

	while(l <= r) {
		if(*l < pivot) {
			++l;
			continue;
		}
		if(*r > pivot) {
			--r;
			continue;
		}
		int t = *l;
		*l++ = *r;
		*r-- = t;	
	}	
	jnxquicksort(array,r - array + 1);
	jnxquicksort(l, array + len - l);
	return 0;
}	
