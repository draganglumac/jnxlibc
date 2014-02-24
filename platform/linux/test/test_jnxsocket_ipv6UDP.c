/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/21/14 18:00:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxsocket.h"
#include <assert.h>
#include <pthread.h>
void test_create()
{
	JNX_LOGC("Test creation of socket\n");
	jnx_socket *a = jnx_socket_tcp_create(AF_INET);
	assert(a);
	jnx_socket *b = jnx_socket_tcp_create(AF_INET6);
	assert(b);
	jnx_socket *c = jnx_socket_udp_create(AF_INET);
	assert(c);
	jnx_socket *d = jnx_socket_udp_create(AF_INET6);
	assert(d);

	jnx_socket_destroy(&a);
	jnx_socket_destroy(&b);
	jnx_socket_destroy(&c);
	jnx_socket_destroy(&d);

	assert(a == NULL);
	assert(b == NULL);
	assert(c == NULL);
	assert(d == NULL);
}
static int tcp_ip4_complete = 0;
static jnx_socket *tcp_ipv4_listener;
static int tcp_ip6_complete = 0;
static jnx_socket *tcp_ipv6_listener;

///////////////////TCP IPV4 /////////////////////////////
void tcplistenipv4_callback(char *msg, size_t size,jnx_socket *s)
{
	assert(strcmp(msg,"hi") == 0);
	tcp_ip4_complete = 1;
}
void *test_tcp_listen_ipv4(void *args)
{
	listen_callback c = tcplistenipv4_callback;
	tcp_ipv4_listener = jnx_socket_tcp_create(AF_INET);
	jnx_socket_tcp_listen(tcp_ipv4_listener,"8831",100,c);
	return 0;
}
void setup_tcp_listen_ipv4_test()
{
	JNX_LOGC("Starting TCP IPV4 test\n");
	pthread_t worker;
	pthread_create(&worker,NULL,test_tcp_listen_ipv4,NULL);

	sleep(2);
	//listener setup lets send a message
	jnx_socket *s = jnx_socket_tcp_create(AF_INET);
	size_t bytes = jnx_socket_tcp_send(s,"localhost","8831","hi",strlen("hi"));	
	jnx_socket_destroy(&s);
	time_t st;
	time(&st);
	int max = 5;
	float elapsed = 0;
	while(tcp_ip4_complete == 0 && elapsed < max)
	{
		
		time_t e;
		time(&e);
		elapsed += (e-st);
	}
	jnx_socket_destroy(&tcp_ipv4_listener);
}

///////////////////TCP IPV6 /////////////////////////////
void tcplistenipv6_callback(char *msg, size_t size,jnx_socket *s)
{
	assert(strcmp(msg,"hi") == 0);
	tcp_ip6_complete = 1;
}
void *test_tcp_listen_ipv6(void *args)
{
	listen_callback c = tcplistenipv6_callback;
	tcp_ipv6_listener = jnx_socket_tcp_create(AF_INET6);
	jnx_socket_tcp_listen(tcp_ipv6_listener,"8831",100,c);
	return 0;
}
void setup_tcp_listen_ipv6_test()
{
	JNX_LOGC("Starting TCP IPV6 test\n");
	pthread_t worker;
	pthread_create(&worker,NULL,test_tcp_listen_ipv6,NULL);

	sleep(2);
	//listener setup lets send a message
	jnx_socket *s = jnx_socket_tcp_create(AF_INET6);
	size_t bytes = jnx_socket_tcp_send(s,"localhost","8831","hi",strlen("hi"));	
	jnx_socket_destroy(&s);
	time_t st;
	time(&st);
	int max = 5;
	float elapsed = 0;
	while(tcp_ip6_complete == 0 && elapsed < max)
	{
		
		time_t e;
		time(&e);
		elapsed += (e-st);
	}
	jnx_socket_destroy(&tcp_ipv6_listener);
}


static int udp_ip4_complete = 0;
jnx_socket *udp_ipv4_listener;
static int udp_ip6_complete = 0;
jnx_socket *udp_ipv6_listener;
/////////////////////UDP IPV4 /////////////////////////////
void udplistenipv4_callback(char *msg, size_t size,jnx_socket *s)
{
	assert(strcmp(msg,"hi") == 0);
	udp_ip4_complete = 1;
}
void *test_udp_listen_ipv4(void *args)
{
	listen_callback c = udplistenipv4_callback;
	udp_ipv4_listener = jnx_socket_udp_create(AF_INET);
	jnx_socket_udp_listen(udp_ipv4_listener,"8831",100,c);
	return 0;
}
void setup_udp_listen_ipv4_test()
{
	JNX_LOGC("Starting UDP IPV4 test\n");
	pthread_t worker;
	pthread_create(&worker,NULL,test_udp_listen_ipv4,NULL);

	sleep(2);
	//listener setup lets send a message
	jnx_socket *s = jnx_socket_udp_create(AF_INET);
	size_t bytes = jnx_socket_udp_send(s,"localhost","8831","hi",strlen("hi"));	
	jnx_socket_destroy(&s);
	time_t st;
	time(&st);
	int max = 5;
	float elapsed = 0;
	while(udp_ip4_complete == 0 && elapsed < max)
	{
		
		time_t e;
		time(&e);
		elapsed += (e-st);
	}
	jnx_socket_destroy(&udp_ipv4_listener);
}
/////////////////////UDP IPV6 /////////////////////////////
void udplistenipv6_callback(char *msg, size_t size,jnx_socket *s)
{
	assert(strcmp(msg,"hi") == 0);
	udp_ip6_complete = 1;
}
void *test_udp_listen_ipv6(void *args)
{
	listen_callback c = udplistenipv6_callback;
	udp_ipv6_listener = jnx_socket_udp_create(AF_INET6);
	jnx_socket_udp_listen(udp_ipv6_listener,"8831",100,c);
	return 0;
}
void setup_udp_listen_ipv6_test()
{
	JNX_LOGC("Starting UDP IPV6 test\n");
	pthread_t worker;
	pthread_create(&worker,NULL,test_udp_listen_ipv6,NULL);

	sleep(2);
	//listener setup lets send a message
	jnx_socket *s = jnx_socket_udp_create(AF_INET6);
	size_t bytes = jnx_socket_udp_send(s,"localhost","8831","hi",strlen("hi"));	
	jnx_socket_destroy(&s);
	
	time_t st;
	time(&st);
	int max = 5;
	float elapsed = 0;
	while(udp_ip6_complete == 0 && elapsed < max)
	{
		
		time_t e;
		time(&e);
		elapsed += (e-st);
	}
	jnx_socket_destroy(&udp_ipv6_listener);
}
int main(int argc, char **argv)
{
	JNX_LOGC("Starting network tests\n");
//	test_create();
//	setup_tcp_listen_ipv4_test();
//	setup_tcp_listen_ipv6_test();
	
//	setup_udp_listen_ipv4_test();
	setup_udp_listen_ipv6_test();
	
	return 0;
}
