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
#include <string.h>
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
	struct foo *output = (struct foo*)jnx_list_remove(&secondlist);
	assert(output->number == 10);
	free(output);
	jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
	printf("- test_list_deletion"); 
	jnx_list_delete(&secondlist);
	jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
void test_list_index()
{
	printf("- test_list_sequence");
	jnx_list *j = jnx_list_init();
	int count = 6;
	char *ar[] = { "A", "B", "C", "D", "E", "F" };
	int y;
	for(y = 0; y < count; ++y)
	{
		jnx_list_add(j,ar[y]);
	}
	int x; 
	for(x = count - 1; x != 0; --x)
	{
		char *current = jnx_list_remove(&j);
		assert(strcmp(current,ar[x]) == 0);	
	}	
	jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
	jnx_list_delete(&j);
	assert(j == NULL);
}
void test_data_removal()
{
	printf("- test_data_removal");
	char *test_string = "AABBCC";
	jnx_list *list = jnx_list_init();
	int count = 10;
	int x;
	for(x = 0; x < count; ++x)
	{
		char *dynamic_string = malloc(strlen(test_string));
		jnx_list_add(list,dynamic_string);
	}
	char *ret;
	while((ret = jnx_list_remove(&list)) != NULL)
	{
		free(ret);
	}
	jnx_list_delete(&list);
	assert(list == NULL);
	jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
void test_list_tail()
{
	jnx_list *l = jnx_list_init();

	int count = 3;
	char *ar[] = { "A", "B", "C" };
	int x;
	for(x = 0; x < count; ++x)
	{
		jnx_list_add(l,ar[x]);
	}
	while(l->tail)
	{
		l->tail = l->tail->prev_node;
	}

	jnx_list_delete(&l);
}
int main(int args, char **argv)
{
	printf("Running list tests...\n");
	test_list_creation();
	test_data_removal();
	test_list_index();
	test_list_tail();
	printf("List tests completed.\n");
	return 0;
}
