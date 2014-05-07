/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/07/14 07:21:26
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
#include "jnxlog.h"
#include "jnxterm.h"
#include <assert.h>
#include <stdio.h>
int main(int argc, char **argv) {
	JNX_LOGC(JLOG_DEBUG,"Starting quicksort tests -");
	int assumed[] = { -41, 4, 5, 32, 100 };
	int a[] = { 4, 100, 5, 32, -41 };


	int n = sizeof a / sizeof a[0];
	jnxquicksort(a,n);

	int x;
	for(x =0;x<n; ++x) {
		assert(a[x] == assumed[x]);
	}

    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
	return 0;
}
