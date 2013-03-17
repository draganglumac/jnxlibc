/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/16/13 11:42:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "../src/jnxvector.h"
#include <stdio.h>
#include <assert.h>
typedef struct{
    int a;
}teststruct;
void test_creation()
{
    printf("test_creation\n");
    jnx_vector *myvector = jnx_vector_init();
    assert(myvector != NULL);
    jnx_vector_delete(myvector);    
}
void test_push()
{
    printf("test_push\n");
    jnx_vector *myvector = jnx_vector_init();
    int x;
    for(x = 0; x < 10; ++x)
    {
        jnx_vector_push(myvector,(void*)"A");
    }
    jnx_vector_delete(myvector);    
}
void test_pop()
{
    printf("test_pop\n");
    teststruct *d = malloc(sizeof(teststruct));
    d->a = 10;
    jnx_vector *myvector = jnx_vector_init();
    jnx_vector_push(myvector,d);
    
    teststruct *ret = (teststruct*)jnx_vector_pop(myvector);
    assert(ret->a == d->a); 
    jnx_vector_delete(myvector);    
    free(d);
}
int main(int argc, char **argv)
{
    test_creation();
    test_push();
    test_pop();
    return 0;
}
