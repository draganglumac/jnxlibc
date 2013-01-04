#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxfile.h"


int main (int argc, char** argv)
{
	jnx_file_recursive_delete("test");
	
    return 0;
}
