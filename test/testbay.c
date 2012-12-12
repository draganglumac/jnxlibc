#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxnetwork.h"

void callback(char* msg)
{
	
}
int main(int argc, char **argv) {

	jnx_listener_callback c = &callback;
	jnx_setup_listener(8080,c);
    return 0;
}
