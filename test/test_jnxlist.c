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

struct foo{
    int number;
}foo;



void looper()
{
    jnx_list *list = jnx_list_make();
    int x,y;
    char alphabet = 'a';
    for(x = 0; x < 26; ++x)
    {
        jnx_list_add(list,(void*)alphabet);
        ++alphabet;
    }
    alphabet = 'a';
    for(y = 0; y < x; ++y)
    {
        assert((char)jnx_list_remove(list) == alphabet);
        ++alphabet;
    }

    jnx_list_delete(list);
}
void structo()
{
    jnx_list *secondlist = jnx_list_make();
    struct foo *f = malloc(sizeof(foo));
    f->number = 10;
    jnx_list_add(secondlist,(void*)f); 
    struct foo *output = (struct foo*)jnx_list_remove(secondlist);
    assert(output->number == 10);
    jnx_list_delete(secondlist);
}
int main(int args, char **argv)
{
    printf("Running test for jnxlist\n");

    looper();
//    structo();

    return 0;
}
