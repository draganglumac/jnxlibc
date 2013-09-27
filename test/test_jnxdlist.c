/*
 * =====================================================================================
 *
 *       Filename:  test_jnxdlist.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/27/13 16:08:22
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
#include "../src/jnxdlist.h"
#include "../src/jnxterm.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	printf("Running dlist tests...\n");
	jnx_dlist *l = jnx_dlist_init();
	int count = 5;
	char *ar[] = { "A" , "B", "C" , "D" , "E" };
	int x;
	for(x = 0; x < count; ++x)
	{
		jnx_dlist_add(l,ar[x]);
	}

	jnx_node *head_marker = l->head;
	while(l->head)
	{
		printf("%s\n",l->head->_data);
		l->head = l->head->next_node;
	}
	l->head = head_marker;
	
	jnx_node *tail_marker = l->tail;
	while(l->tail)
	{
		printf("%s\n",l->tail->_data);
		l->tail = l->tail->prev_node;
	}
	jnx_dlist_delete(l);
	jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
	printf("Double link List tests completed.\n");
	return 0;
}
