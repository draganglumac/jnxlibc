#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "src/network.h"

void callback(char*msg)
{
    printf("callback says -> %s\n",msg);
}

int main(int argc, char **argv) {


    Callback c = &callback;
    setup_listener(6001,c);


    return 0;
}
