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
#include "jnxencoder.h"
#include <string.h>
#include <stdio.h>
#include "jnxterm.h"
#include "jnxcheck.h"
#include "jnxlog.h"
int main(int argc, char **argv) {

    JNX_LOGC(JLOG_DEBUG,"Running test for jnxencoder\n");

	jnx_encoder *e = jnx_encoder_create();

	char *test[] = {"A","BA","Some String","AbbA","A A A ","","Matey"};
	int x;

	for(x = 0; x < 5; ++x) {
		size_t test_len;
		char *encoded_test = jnx_encoder_b64_encode(e,test[x],strlen(test[x]),&test_len);
		size_t decode_len;

		char *decoded_test = jnx_encoder_b64_decode(e,encoded_test,test_len,&decode_len);
		JNXCHECK(strcmp(decoded_test,test[x]) == 0);
		free(encoded_test);
	}
	jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");

	jnx_encoder_destroy(&e);
	JNXCHECK(e == NULL);
	return 0;
}
