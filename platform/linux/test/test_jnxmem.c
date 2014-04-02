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
#include "jnxlog.h"
#include "jnxterm.h"
#include "jnxfile.h"
#include "jnxlist.h"
#include "jnxmem.h"
#include <assert.h>
void test_basic() {
    JNX_LOGC(JLOG_DEBUG,"- test_basic");
    int x;
    int y = 10;
    char *ar[y];
    for(x=0; x<y; ++x) {
        char *s = JNX_MEM_MALLOC(sizeof(char));
        assert(jnx_mem_get_total_number_allocations() == x + 1);
        ar[x] = s;
    }

    assert(jnx_mem_get_total_size_allocations() == (sizeof(char) * 10));
    assert(jnx_mem_get_current_number_allocations() == 10);
    assert(jnx_mem_get_current_size_allocations() == (sizeof(char) *10));
    for(x=0; x<y; ++x) {
        JNX_MEM_FREE(ar[x]);
    }
    assert(jnx_mem_get_current_number_allocations() == 0);
    assert(jnx_mem_get_current_size_allocations() == 0);

    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_allocation_times() {
    JNX_LOGC(JLOG_DEBUG,"- test_allocation_times\n");

    int l = 3;
    int ar[3] = { 100, 1000, 10000 };
    int c,d=0;

    for(d=0; d < l; ++d) {
        clock_t start = clock();
        for(c=0; c<ar[d]; ++c) {
            char *s = JNX_MEM_MALLOC(sizeof(char));
        }
        clock_t end = clock();
        JNX_LOGC(JLOG_DEBUG,"- Allocated %d blocks of %d in %zu\n",ar[d],sizeof(char),(end - start));
        assert(jnx_mem_get_current_number_allocations() == ar[d]);
        size_t rb = jnx_mem_clear();
        JNX_LOGC(JLOG_DEBUG,"- Cleared %d blocks\n",rb);
        assert(rb == (ar[d] * sizeof(char)));
        assert(jnx_mem_get_total_number_allocations() == 0);
        assert(jnx_mem_get_current_number_allocations() == 0);
        assert(jnx_mem_get_total_size_allocations() == 0);
        assert(jnx_mem_get_current_size_allocations() == 0);
    }
}
void test_deallocation_times() {
    JNX_LOGC(JLOG_DEBUG,"- test_deallocation_times\n");
    int l = 3;
    int ar[3] = { 100, 1000, 10000};
    int c,d;
    char *h = calloc(3, sizeof(char));
	for(c = 0; c < l; ++c) {
		
		int current_number = ar[c];
		
		void *ptr_array[current_number];

		for(d = 0; d < current_number; ++d) {
			
			ptr_array[d] = JNX_MEM_MALLOC(sizeof(char));
		}

		clock_t start = clock();	
		for(d = 0; d < current_number; ++d) {

			JNX_MEM_FREE(ptr_array[d]);
		}
		clock_t end = clock();
		JNX_LOGC(JLOG_DEBUG,"- Dealloced %d blocks of %d in %zu\n",current_number, sizeof(char), (end - start));
	}
        assert(jnx_mem_get_current_number_allocations() == 0);
        assert(jnx_mem_get_current_size_allocations() == 0);
}
int main(int argc, char **argv) {
#if defined(JNX_MEMORY_MANAGEMENT)
    JNX_LOGC(JLOG_DEBUG,"Running memory tests...\n");
    test_basic();
    test_allocation_times();
    test_deallocation_times();
    JNX_LOGC(JLOG_DEBUG,"Memory tests completed\n");
#else
    JNX_LOGC(JLOG_DEBUG,"JNX_MEMORY_MANAGEMENT not defined - skipping tests\n");
#endif
    return 0;
}
