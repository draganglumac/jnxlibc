/*
 * =====================================================================================
 *
 *       Filename:  test_jnxdebug.c
 *
 *    Description:  Memory manager tests
 *
 *        Version:  1.0
 *        Created:  12/06/13 08:22:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesaax@hush.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include "jnxterm.h"
#include "jnxfile.h"
#include "jnxlist.h"
#include "jnxmem.h"
#include <assert.h>
void test_basic()
{
	printf("- test_basic");
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");


	int x;
	int y = 10;
	char *ar[y];
	for(x=0;x<y;++x)
	{
		char *s = JNX_MEM_MALLOC(sizeof(char));
		assert(jnx_mem_get_total_number_allocations() == x + 1);	
		ar[x] = s;
	}
    for(x=0;x<y;++x)
	{
		JNX_MEM_FREE(ar[x]);
	}
	
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
int main(int argc, char **argv)
{
	printf("Running memory tests...\n");
#if defined(DEBUG) || defined(Debug)
	test_basic();
#else
#endif
	printf("Memory tests completed\n");
	return 0;
}
