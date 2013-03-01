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

    jnx_list_add(list,"Alex");

    char *ret = jnx_list_remove(list);

    assert(ret == "Alex");

    jnx_list_delete(list);
    
    return 0;
}
