#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxlist.h"


int main (int argc, char** argv)
{
	jnx_list *mylist = jnx_list_make();
	jnx_list_add(mylist,"A");
	printf("%s\n",jnx_list_remove(mylist));
	
	
  return 0;
}
