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
	int x;
	int y = 10;
	char *ar[y];
	for(x=0;x<y;++x)
	{
		char *s = JNX_MEM_MALLOC(sizeof(char));
		assert(jnx_mem_get_total_number_allocations() == x + 1);	
		ar[x] = s;
	}

	assert(jnx_mem_get_total_size_allocations() == (sizeof(char) * 10));
	assert(jnx_mem_get_current_number_allocations() == 10);
	assert(jnx_mem_get_current_size_allocations() == (sizeof(char) *10));	
	for(x=0;x<y;++x)
	{
		JNX_MEM_FREE(ar[x]);
	}
	assert(jnx_mem_get_current_number_allocations() == 0);
	assert(jnx_mem_get_current_size_allocations() == 0);	

	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_allocation_times()
{
	printf("- test_allocation_times\n");

	int l = 3;
	int ar[3] = { 100, 1000, 10000 };
	int c,d=0;

	for(d=0; d < l; ++d)
	{	
		clock_t start = clock();
		for(c=0;c<ar[d]; ++c)
		{
			char *s = JNX_MEM_MALLOC(sizeof(char));
		}
		clock_t end = clock();
		printf("- Allocated %d blocks of %d in %zu\n",ar[d],sizeof(char),(end - start));
		assert(jnx_mem_get_current_number_allocations() == ar[d]);
		size_t rb = jnx_mem_clear();
		printf("- Cleared %d blocks\n",rb);
		assert(rb == (ar[d] * sizeof(char)));
		assert(jnx_mem_get_total_number_allocations() == 0);
		assert(jnx_mem_get_current_number_allocations() == 0);
		assert(jnx_mem_get_total_size_allocations() == 0);
		assert(jnx_mem_get_current_size_allocations() == 0);
	}
}
void test_deallocation_times()
{
	printf("- test_deallocation_times\n");
	int l = 3;
	int ar[3] = { 100, 1000, 10000};
	int c,d;
	char **h = calloc(3, sizeof(char));
	for(c=0;c<l;++c)
	{
		h[c] = calloc(ar[c],sizeof(char));
	}
	for(c=0;c<l;++c)
	{
		for(d=0;d<ar[l];++d)
		{
			char *s = JNX_MEM_MALLOC(sizeof(char));
			h[c][d] = s;
		}
	}
	for(c=0;c<l;++c)
	{
		clock_t start = clock();
		for(d=0;d<ar[c];++d)
		{
			JNX_MEM_FREE(h[c][d]);
		}
		clock_t end = clock();
		printf("- Deallocated %d blocks of %d in %zu\n",ar[c],sizeof(char),(end - start));
		assert(jnx_mem_get_current_number_allocations() == 0);
		assert(jnx_mem_get_current_size_allocations() == 0);
	}
}
int main(int argc, char **argv)
{
#if defined(JNX_MEMORY_MANAGEMENT)
	printf("Running memory tests...\n");
	test_basic();
	test_allocation_times();
	test_deallocation_times();
	printf("Memory tests completed\n");
#else
	printf("JNX_MEMORY_MANAGEMENT not defined - skipping tests\n");
#endif
	return 0;
}
