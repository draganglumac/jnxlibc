/*
 * =====================================================================================
 *
 *       Filename:  test_jnxnetwork.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/23/13 16:06:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "../src/jnxnetwork.h"
#include <pthread.h>
#include <stdio.h>
#include <assert.h>
void network_listener_callback(char *message)
{
    assert(strcmp(message,"Hello there") == 0);
    exit(0);
}
void *worker_thread(void*args)
{
    jnx_network_broadcast_callback c = &network_listener_callback;
    jnx_network_broadcast_listener(12345,"225.0.0.37",c);    
}
int main(int argc, char **argv)
{
    pthread_t threadone;

    pthread_create(&threadone, NULL, &worker_thread, NULL); 
    sleep(1);
    jnx_network_send_broadcast(12345,"225.0.0.37","Hello there");
    sleep(2);
    return 0;
}
