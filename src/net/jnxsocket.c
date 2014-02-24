/*
 * =====================================================================================
 *
 *       Filename:  jnxsocket.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/21/14 18:07:02
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
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxsocket.h"
#define MAXBUFFER 1024
jnx_socket *create_socket(unsigned int type,unsigned int addrfamily)
{
	assert(type);
	assert(addrfamily);
	int sock = socket(addrfamily,type,0);
	if(sock < 0)
	{
		return NULL;
	}
	int optval = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof optval);
	jnx_socket *s = malloc(sizeof(jnx_socket));
	s->isclosed = 0;
	s->addrfamily = addrfamily;
	s->socket = sock;
	s->stype = type;
	return s;
}
jnx_socket *jnx_socket_tcp_create(unsigned int addrfamily)
{
	return create_socket(SOCK_STREAM,addrfamily);
}
jnx_socket *jnx_socket_udp_create(unsigned int addrfamily)
{
	return create_socket(SOCK_DGRAM,addrfamily);
}
void jnx_socket_close(jnx_socket *s)
{
	if(!s->isclosed)
	{
		if(s->socket != -1)
		{
			close(s->socket);
			s->isclosed = 1;
		}
	}
}
void jnx_socket_destroy(jnx_socket **s)
{
	jnx_socket_close(*s);
	free(*s);
	*s = NULL;	
}
size_t jnx_socket_udp_enable_broadcast(socket *s)
{
	assert(s->stype == SOCK_DGRAM);
	assert(s->addrfamily == AF_INET);
	int optval = 1;
	if(setsockopt(s->socket,SOL_SOCKET,SO_BROADCAST,&optval, sizeof(optval)) != 0)
	{
		perror("setsockopt:");
		return -1;
	}
	return 0;
}
size_t jnx_socket_tcp_send(jnx_socket *s, char *host, char* port, char *msg, ssize_t msg_len)
{
	assert(s);
	assert(host);
	assert(port);
	assert(msg);
	assert(msg_len);
	assert(s->isclosed == 0);
	assert(s->stype == SOCK_STREAM);
	struct addrinfo hints, *res;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = s->addrfamily;
	hints.ai_socktype = s->stype;

	getaddrinfo(host,port,&hints,&res);

	if(connect(s->socket,res->ai_addr,res->ai_addrlen) != 0)
	{
		perror("connect:");
		freeaddrinfo(res);
		return -1;
	}
	freeaddrinfo(res);

	size_t tbytes = 0;
	size_t rbytes = msg_len;

	while(tbytes < rbytes)
	{
		size_t n = write(s->socket,msg,rbytes);
		if(n == -1)
		{
			perror("send:");
			return -1;
		}
		tbytes +=n;
		rbytes = msg_len - tbytes;
	}	
	return tbytes;
}
size_t jnx_socket_udp_send(jnx_socket *s, char *host, char* port, char *msg, ssize_t msg_len)
{
	assert(s);
	assert(host);
	assert(port);
	assert(msg);
	assert(msg_len);
	assert(s->isclosed == 0);
	assert(s->stype == SOCK_DGRAM);
	struct addrinfo hints, *res;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = s->addrfamily;
	hints.ai_socktype = s->stype;
	hints.ai_flags =0;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_next = NULL;	

	getaddrinfo(host,port,&hints,&res);

	size_t tbytes = 0;
	size_t rbytes = msg_len;

	while(tbytes < rbytes)
	{
		size_t n = sendto(s->socket,msg,msg_len,0,res->ai_addr,res->ai_addrlen);
		if(n == -1)
		{
			perror("send:");
			return -1;
		}
		tbytes +=n;
		rbytes = msg_len - tbytes;
	}	
	return tbytes;
}
size_t jnx_socket_tcp_listen(jnx_socket *s, char* port, ssize_t max_connections, listen_callback c)
{
	assert(s);
	assert(port);
	assert(s->isclosed == 0);
	assert(s->stype == SOCK_STREAM);
	int optval = 1;
	struct addrinfo hints, *res, *p;
	char buffer[MAXBUFFER];
	memset(&hints,0,sizeof(hints));
	hints.ai_family = s->addrfamily;
	hints.ai_socktype = s->stype;

	getaddrinfo(NULL,port,&hints,&res);

	p = res;
	while(p != NULL)
	{
		if((s->socket = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}
		if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &optval,
					sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(s->socket, p->ai_addr, p->ai_addrlen) == -1) {
			perror("server: bind");
			continue;
		}
		break;

		p = p->ai_next;
	}
	freeaddrinfo(res);
	listen(s->socket,max_connections);
	while(1)
	{
		struct sockaddr_storage their_addr;
		socklen_t addr_size = sizeof(their_addr);
		int recfd = accept(s->socket,(struct sockaddr*)&their_addr,&addr_size);
		if(recfd < 0)
		{
			JNX_LOGC("accept: %s",strerror(errno));
			return -1;
		}
		bzero(buffer,MAXBUFFER);
		FILE *fp = tmpfile();
		size_t bytesread = read(recfd,buffer,MAXBUFFER);
		fwrite(buffer,sizeof(char),bytesread,fp);

		while(bytesread > 0)
		{
			bzero(buffer,MAXBUFFER);
			bytesread = read(recfd,buffer,MAXBUFFER);
			if(bytesread == -1)
			{
				perror("read:");
				fclose(fp);
				return -1;
			}
			if(bytesread > 0)
			{
				fwrite(buffer,sizeof(char),bytesread,fp);
			}
		}
		int len = ftell(fp);
		rewind(fp);
		char *out = calloc(len + 1, sizeof(char));
		fread(out,1,len,fp);
		fclose(fp);
		c(out,len,s);
	}
	return 0;
}
size_t jnx_socket_udp_listen(jnx_socket *s, char* port, ssize_t max_connections, listen_callback c)
{
	assert(s);
	assert(port);
	assert(s->isclosed == 0);
	assert(s->stype == SOCK_DGRAM);
	int optval = 1;
	struct addrinfo hints, *res, *p;
	struct sockaddr_storage their_addr;
	socklen_t addr_len;
	char buffer[MAXBUFFER];
	memset(&hints,0,sizeof(hints));
	hints.ai_family = s->addrfamily;
	hints.ai_socktype = s->stype;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL,port,&hints,&res);

	p = res;
	while(p != NULL)
	{
		if((s->socket = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}
		if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &optval,
					sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(s->socket, p->ai_addr, p->ai_addrlen) == -1) {
			perror("server: bind");
			continue;
		}
		break;
		p= p->ai_next;
	}
	freeaddrinfo(res);

	while(1){
		addr_len = sizeof(their_addr);
		bzero(buffer,MAXBUFFER);
		size_t tbytes = recvfrom(s->socket,buffer,MAXBUFFER,0,(struct sockaddr *)&their_addr,
				&addr_len);
		if(tbytes == -1)
		{
			perror("recvfrom:");
			return -1;
		}
		c(buffer,tbytes,s);
	}
	return -1;
}
