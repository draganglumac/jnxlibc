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
#include "../src/jnxterm.h"
int main(int args, char **argv)
{
    printf("- test_jnxfile");
  
    char buffer[1024];
    strcpy(buffer,"K=V");
    jnx_file_write("test_jnxfile.txt",buffer);

    char *output = jnx_file_read("test_jnxfile.txt");

    assert(strcmp(buffer,output) == 0);
    
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
    system("rm test_jnxfile.txt");

    printf("File tests completed.\n");
    return 0;
}
