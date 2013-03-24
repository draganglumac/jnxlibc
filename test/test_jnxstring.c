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
#include "../src/jnxstring.h"
#include "../src/jnxterm.h"
int main(int args, char **argv)
{
    printf("Running test for jnxstring\n");
  
    printf("- test_string_join"); 
    char *temp = malloc(256);
    strcpy(temp,"foo"); 
    jnx_string_join(&temp,"bar");
    assert(strcmp(temp,"foobar") == 0);
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
    printf("String tests completed\n");
    return 0;
}
