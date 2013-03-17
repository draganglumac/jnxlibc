/*
 * =====================================================================================
 *
 *       Filename:  bob.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/17/2013 12:20:11 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int bob[] = { 1, 2, 3, 4, 5 };
    int jim[3], i, j;

    memmove((void *)jim, (const void *)(bob + 1), 3 * sizeof(int));
    bzero((void *)(bob + 1), 3 * sizeof(int));

    printf("bob = [");
    for ( i = 0; i < 5; i++ )
       printf("%d, ", bob[i]);
    printf("]\n");

    printf("jim = [");
    for ( j = 0; j < 3; j++ )
       printf("%d, ", jim[j]);
    printf("]\n");

    return 0;
}
