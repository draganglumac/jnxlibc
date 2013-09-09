/*
 * =====================================================================================
 *
 *       Filename:  test_jnxterm.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/28/2013 08:59:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <assert.h>
#include "../src/jnxterm.h"
#include "../src/jnxfile.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
void test_redirect()
{
	char *test_path = "console_redirect.log";
	system("touch console_redirect.log");
	jnx_term_override_stdout(test_path);
	system("echo 'test'");
	jnx_term_reset_stdout();
	char *buffer;
	size_t read = jnx_file_read(test_path,&buffer);
	system("rm console_redirect.log");
	buffer[strlen(buffer) - 1] = '\0';
	assert(strcmp(buffer,"test") == 0);
}
int main()
{
    printf("Running jnx_term tests\n");
    printf("- terminal tests ");
	test_redirect();
	jnx_term_printf_in_color(JNX_COL_GREEN,"OK\n");
	printf("- terminal output ");
	jnx_term_load_bar(1);
	sleep(2);
	jnx_term_load_bar(0);
	jnx_term_load_spinner(1);
	sleep(2);
	jnx_term_load_spinner(0);
	jnx_term_printf_in_color(JNX_COL_GREEN,"OK\n");
	printf("Terminal tests completed.\n");
    return 0;
}
