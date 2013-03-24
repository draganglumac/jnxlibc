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
#include <stdio.h>
int main()
{
    printf("Running jnx_term tests\n");
    printf("- terminal tests");
    jnx_term_printf_in_color(JNX_COL_YELLOW, "  NOT IMPLEMENTED\n");
    jnx_term_default();
    printf("Terminal tests completed.\n");
    return 0;
}
