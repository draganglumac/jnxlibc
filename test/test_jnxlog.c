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
#include <string.h>
#include "jnxlog.h"
#include "jnxterm.h"
int main(int args, char **argv)
{
    printf("Running test for jnxlog\n");
    printf("- test_log_insertion");
    assert(jnx_log_setup("test.log") == 0);
    jnx_log("Hello from log!\n");
    jnx_log("Hello from log with %s %s formatted args %d\n","lots","of",3);
    system("rm test.log");
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
    return 0;
}
