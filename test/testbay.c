#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxnetwork.h"

void callback(char* msg)
{
	
}
int main(int argc, char **argv) {

	printf("%s\n",jnx_local_ip("wlan0"));
    return 0;
}
