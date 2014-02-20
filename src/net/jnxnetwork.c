/*
 * =====================================================================================
 *
 *       Filename:  jnxnetwork.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/20/14 12:51:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "jnxnetwork.h"
#include "jnxlog.h"
#define MAXBUFFER 1024
void jnx_network_socket_destroy(jnx_socket *s)
{
	close(s->socket);
	free(s);
}
jnx_socket *jnx_network_socket_create(unsigned int addrfamily,ssize_t stype)
{
	int sockfd = socket(addrfamily,stype,0);
	int optval = 1;
	if(socket < 0)
	{
		JNX_LOGC("Could not create socket\n");
		return NULL;
	}
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR, &optval, sizeof optval) > 0)
	{
		JNX_LOGC("Could not set socket options\n");
		close(sockfd);
		return NULL;
	}
	jnx_socket *s = malloc(sizeof(jnx_socket));
	s->addrfamily = addrfamily;
	s->stype = stype;
	s->socket = sockfd;
	return s;
}
size_t jnx_network_send(jnx_socket *s, char *host, ssize_t port, char *msg, ssize_t msg_len)
{
	assert(host);
	assert(port);
	assert(msg);

	//IPV4
	struct sockaddr_in conn_addr;
	//IPV6
	struct sockaddr_in6 conn_addr6;

	struct hostent *conn = gethostbyname(host);
	if(conn < 0)
	{
		JNX_LOGC("Failed to get host by name\n");
		return -1;
	}
	//IPV4
	bzero((char*) &conn_addr, sizeof(conn_addr));
	conn_addr.sin_family = s->addrfamily;
	bcopy((char*)conn->h_addr, (char*) &conn_addr.sin_addr.s_addr,conn->h_length);
	conn_addr.sin_port = htons(port);
	//IPV6
	bzero((char*) &conn_addr6, sizeof(conn_addr6));
	conn_addr6.sin6_family = s->addrfamily;
	conn_addr6.sin6_addr = in6addr_loopback;
	conn_addr6.sin6_port = htons(port);

	switch(s->addrfamily)
	{
		case AF_INET6:
			if(connect(s->socket,(struct sockaddr*) &conn_addr6, sizeof(conn_addr6)) < 0)
			{
				JNX_LOGC("Failed to connect IPV6 %s",strerror(errno));
				return -1;
			}
			break;
		default:
			if(connect(s->socket,(struct sockaddr*) &conn_addr, sizeof(conn_addr)) < 0)
			{
				JNX_LOGC("Failed to connect IPV4 %s",strerror(errno));
				return -1;
			}
	}
	ssize_t n = write(s->socket,msg,strlen(msg));
	if(n < 0)
	{
		JNX_LOGC("Could not write successfully\n");
		return -1;
	}
	return n;
}
size_t jnx_network_listen(jnx_socket *s, ssize_t port, ssize_t max_connections, listen_callback c)
{
	assert(port);
	assert(max_connections);

	///IPV4
	struct sockaddr_in conn_addr;
	bzero((char*)&conn_addr,sizeof(conn_addr));
	conn_addr.sin_addr.s_addr = INADDR_ANY;
	conn_addr.sin_family = s->addrfamily;
	conn_addr.sin_port = htons(port);


	///IPV6
	struct sockaddr_in6 conn_addr6;
	bzero((char*)&conn_addr6,sizeof(conn_addr6));
	conn_addr6.sin6_addr = in6addr_any;	
	conn_addr6.sin6_family = s->addrfamily;
	conn_addr6.sin6_port = htons(port);	

	switch(s->addrfamily)
	{
		case AF_INET6:
			if(bind(s->socket,(struct sockaddr*) &conn_addr6, sizeof(conn_addr6)) < 0)
			{
				JNX_LOGC("Error binding IPV6 socket - %s\n",strerror(errno));
				return -1;
			}
			break;
		default:
			if(bind(s->socket,(struct sockaddr*) &conn_addr, sizeof(conn_addr)) < 0)
			{
				JNX_LOGC("Error binding IPV4 socket - %s\n",strerror(errno));
				return -1;
			}
			break;
	}

	listen(s->socket,max_connections);
	//IPV4
	struct sockaddr_in cli_addr;
	///IPV6
	struct sockaddr_in6 cli_addr6;

	int n = -1;
	while(1)
	{
		int clilen = 0;
		int clisock;
		switch(s->addrfamily)
		{
			case AF_INET6:
				clilen = sizeof(cli_addr6);
				clisock = accept(s->socket,(struct sockaddr*)&cli_addr6,(socklen_t*)&clilen);
				if(clisock < 0){
					JNX_LOGC("Error on IPV6 accept\n");
					return -1;
				}
				break;
			default:
				clilen = sizeof(cli_addr);
				clisock = accept(s->socket,(struct sockaddr*)&cli_addr,(socklen_t*)&clilen);
				if(clisock < 0){
					JNX_LOGC("Error on IPV4 accept\n");
					return -1;
				}
				break;
		}
		FILE *tmp_file = tmpfile();
		assert(tmp_file);
		size_t bytes = 0;
		while(n != 0)
		{
			char buffer[MAXBUFFER];
			n = read(clisock,buffer,MAXBUFFER);
			bytes += n;
			fwrite(buffer,1,n,tmp_file);
		}
		int l = ftell(tmp_file);
		rewind(tmp_file);
		char *out = calloc(l +1, sizeof(char));
		fread(out,1,l,tmp_file);
		fclose(tmp_file);

		if(s->addrfamily == AF_INET)
		{
			char ip4[INET_ADDRSTRLEN];
			struct sockaddr_in sa;
			inet_ntop(AF_INET,&(sa.sin_addr),ip4,INET_ADDRSTRLEN);
			c(out,l,ip4);
		}else if(s->addrfamily == AF_INET6)
		{
			char ip6[INET6_ADDRSTRLEN];
			struct sockaddr_in6 sa6;
			inet_ntop(AF_INET6,&(sa6.sin6_addr),ip6,INET6_ADDRSTRLEN);
			c(out,l,ip6);
		}else
		{
			c(out,n,"UNKNOWN");
		}
		n = -1;
	}
	return 0;
}

size_t jnx_network_broadcast(jnx_socket *s, ssize_t port, char *group, char *msg,ssize_t msg_len)
{
	struct sockaddr_in cli_addr;
	memset(&cli_addr,0,sizeof(cli_addr));
	cli_addr.sin_family = s->addrfamily;
	cli_addr.sin_addr.s_addr = inet_addr(group);
	cli_addr.sin_port = htons(port);

	if(sendto(s->socket,msg,msg_len,0,(struct sockaddr*)&cli_addr,sizeof(cli_addr)) < 0)
	{
		JNX_LOGC("Error on broadcast\n");
		return -1;
	}
	return 0;
}
size_t jnx_network_broadcast_listen(jnx_socket *s, ssize_t port, char *group, broadcast_listen_callback c)
{
	struct ip_mreq mreq;
	struct sockaddr_in cli_addr;
	memset(&cli_addr,0,sizeof(cli_addr));
	cli_addr.sin_family = s->addrfamily;
	cli_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
	cli_addr.sin_port = htons(port);
	if(bind(s->socket,(struct sockaddr*)&cli_addr,sizeof(cli_addr)) < 0)
	{
		JNX_LOGC("Error on broadcast listen\n");
		return -1;
	}
	mreq.imr_multiaddr.s_addr = inet_addr(group);
	mreq.imr_interface.s_addr = htons(INADDR_ANY);

	if(setsockopt(s->socket,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0)
	{
		JNX_LOGC("Unable to setup broadcast options\n");
		return -1;
	}
	char buffer[MAXBUFFER];
	while(1)
	{
		bzero(buffer,MAXBUFFER);
		int addrlen = sizeof(cli_addr);
		size_t nbytes = recvfrom(s->socket,buffer,MAXBUFFER,0,(struct sockaddr*)&cli_addr,(socklen_t*)&addrlen);
		if(nbytes < 0)
		{
			JNX_LOGC("Error receiving data from broadcast\n");
			return -1;
		}
		c(buffer,nbytes);
	}
}



