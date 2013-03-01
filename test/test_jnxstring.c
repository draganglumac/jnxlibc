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
int main(int args, char **argv)
{
    printf("Running test for jnxstring\n");
   
    char *temp = malloc(256);
    strcpy(temp,"foo"); 
    
    jnx_string_join(&temp,"bar");
    
    assert(strcmp(temp,"foobar") == 0);

    return 0;
}
