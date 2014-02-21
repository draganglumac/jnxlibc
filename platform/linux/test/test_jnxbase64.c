/*
 * =====================================================================================
 *
 *       Filename:  test_jnxbase64.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  07/10/13 13:39:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxbase64.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "jnxterm.h"
int main(int argc, char **argv)
{
	printf("Running base64 tests...");
	char *test[] = {"A","BA","Some String","AbbA","A A A ","","Matey"};
	int x;
	for(x = 0; x < 5; ++x)
	{
		size_t test_len;
		char *encoded_test = jnx_base64_encode(test[x],strlen(test[x]),&test_len);
		size_t decode_len;

		char *decoded_test = jnx_base64_decode(encoded_test,test_len,&decode_len);
		assert(strcmp(decoded_test,test[x]) == 0);
		free(encoded_test);
	}
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
	return 0;
}
