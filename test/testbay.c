#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxfile.h"


int main (int argc, char** argv)
{
	struct jnx_file_kvp_node *head = jnx_file_read_keyvaluepairs("config.conf");
	struct jnx_file_kvp_node *node = head;
	
	printf("Got node\n");
	
	while(node != NULL)
	{
		printf("Key %s Value %s\n",node->key,node->value);
		node = node->next;
	}
	
    return 0;
}
