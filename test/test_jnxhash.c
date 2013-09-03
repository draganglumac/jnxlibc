/*
 * =====================================================================================
 *
 *       Filename:  test_jnxhash.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/27/13 16:20:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../src/jnxhash.h"
#include "../src/jnxterm.h"
int main(int args, char **argv)
{
    printf("Running test for jnxhash\n");
    
    //setup
	printf("- test_hash_creation");
	jnx_hashmap *testhash = jnx_hash_init(1024); 
	assert(testhash);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
    printf("- test_hash_insertion");    
	jnx_hash_put(testhash,"Alex","Jones");
    jnx_hash_put(testhash,"Bob","Marley");
    assert((char*)jnx_hash_get(testhash,"Alex") == "Jones"); 
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
	printf("- test_hash_get_keys");
	const char **buffer;
	int number_of_keys = jnx_hash_get_keys(testhash,&buffer);
	assert(number_of_keys == 2);
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
	free(buffer);
	jnx_hash_delete(testhash);  
	printf("Hash tests completed.\n"); 
    return 0;
}
