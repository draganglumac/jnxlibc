#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxnetwork.h"


int main(int argc, char **argv) {

	jnx_send_message("172.20.141.97","9111","pwd#cwd#ls -la");
    return 0;
}
