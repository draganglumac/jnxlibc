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
#include "../src/jnxlist.h"
int main(int args, char **argv)
{
    printf("Running test for jnxlist\n");
    
    jnx_list *list = jnx_list_make();
    int x,y;
    char alphabet = 'a';
    for(x = 0; x < 26; ++x)
    {
        jnx_list_add(list,(void*)alphabet);
        ++alphabet;
    }
    for(y = 0; y < x; ++y)
    {
        jnx_list_remove(list);
        --alphabet;
    }



    return 0;
}
