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
#include "../src/jnxfile.h"
int main(int args, char **argv)
{
    printf("Running test for jnxfile\n");
  
    char buffer[1024];
    strcpy(buffer,"K=V");
    jnx_file_write("test_jnxfile.txt",buffer);

    char *output = jnx_file_read("test_jnxfile.txt");

    assert(strcmp(buffer,output) == 0);

    jnx_file_kvp_node *node = jnx_file_read_keyvaluepairs("test_jnxfile.txt","=");

    
    system("rm test_jnxfile.txt");

    return 0;
}
