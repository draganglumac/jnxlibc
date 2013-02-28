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

int main()
{
    jnx_printf_in_color(JNX_COL_YELLOW, "Bob's your uncle!\n");
    jnx_printf_in_color(JNX_COL_BLUE, "Here's some formatted parameters: %d, %s\n", 42, "The meaning of life.");

    int i;
    for ( i = 0; i < 7; i++)
    {
        jnx_printf_in_color(i, "I am colour %d\n", i);
    }

    jnx_term_color(JNX_COL_MAGENTA);
    printf("Let's print some lines in magenta.\n");
    printf("And another one.\n");
    printf("And one with some formats %d %c\n", 2, 'B');
    jnx_term_default();

    return 0;
}
