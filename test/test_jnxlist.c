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
#include "../src/jnxterm.h"
struct foo{
    int number;
}foo;

void test_list_creation()
{
    printf("- test_list_creation");
    jnx_list *secondlist = jnx_list_init();
    assert(secondlist != NULL);
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
    struct foo *f = malloc(sizeof(foo));
    f->number = 10;
    printf("- test_list_insertion"); 
    jnx_list_add(secondlist,(void*)f); 
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
    printf("- test_list_removal");
    struct foo *output = (struct foo*)jnx_list_remove(secondlist);
    assert(output->number == 10);
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
    printf("- test_list_deletion"); 
    jnx_list_delete(secondlist);
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
    printf("- test_list_deletion"); 
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
int main(int args, char **argv)
{
    printf("Running list tests...\n");
    test_list_creation();
    printf("List tests completed.\n");
    return 0;
}
