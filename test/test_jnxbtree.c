/*
 * =====================================================================================
 *
 *       Filename:  test_jnxbplustree.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/05/2013 06:29:42 PM
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

#include "../src/jnxbtree.h"

int compare_ints(void *first, void *second)
{
    int *n1 = (int *) first;
    int *n2 = (int *) second;

    if ( *n1 == *n2 )
    {
        return 0;
    }
    else if ( *n1 < *n2 )
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

int main()
{
    jnx_B_tree *tree = jnx_B_tree_init(3, compare_ints);

    return 0;
}
