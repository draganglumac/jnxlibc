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
void jnx_network_socket_close(jnx_socket *s)
{
	if(!s->isclosed)
	{
		close(s->socket);
		s->isclosed =1;
	}
}
void jnx_network_socket_destroy(jnx_socket *s)
{
	jnx_network_socket_close(s);
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
	s->isclosed = 0;
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
	switch(s->addrfamily)
	{
		case AF_INET6:
			bzero((char*) &conn_addr6, sizeof(conn_addr6));
			conn_addr6.sin6_family = s->addrfamily;
			conn_addr6.sin6_addr = in6addr_loopback;
			conn_addr6.sin6_port = htons(port);

			if(s->stype != SOCK_DGRAM){	
				if(connect(s->socket,(struct sockaddr*) &conn_addr6, sizeof(conn_addr6)) < 0)
				{
					JNX_LOGC("Failed to connect IPV6 %s",strerror(errno));
					return -1;
				}
			}
			break;
		default:
			bzero((char*) &conn_addr, sizeof(conn_addr));
			conn_addr.sin_family = s->addrfamily;
			bcopy((char*)conn->h_addr, (char*) &conn_addr.sin_addr.s_addr,conn->h_length);
			conn_addr.sin_port = htons(port);

			if(s->stype != SOCK_DGRAM){	
				if(connect(s->socket,(struct sockaddr*) &conn_addr, sizeof(conn_addr)) < 0)
				{
					JNX_LOGC("Failed to connect IPV4 %s",strerror(errno));
					return -1;
				}
			}
	}
	ssize_t n = 0;
	switch(s->stype)
	{
		case SOCK_STREAM:
			n= write(s->socket,msg,strlen(msg));
			break;
		case SOCK_DGRAM:
			switch(s->addrfamily)
			{
				case AF_INET6:
					n=sendto(s->socket,msg,strlen(msg),0,(struct sockaddr*)&conn_addr, sizeof(conn_addr));
					break;
				case AF_INET:
					n=sendto(s->socket,msg,strlen(msg),0,(struct sockaddr*)&conn_addr6, sizeof(conn_addr6));
					break;
			}
			break;
	}
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

	struct sockaddr_in conn_addr;
	struct sockaddr_in6 conn_addr6;

	switch(s->addrfamily)
	{
		case AF_INET6:
			bzero((char*)&conn_addr6,sizeof(conn_addr6));
			conn_addr6.sin6_addr = in6addr_any;	
			conn_addr6.sin6_family = s->addrfamily;
			conn_addr6.sin6_port = htons(port);	
			if(bind(s->socket,(struct sockaddr*) &conn_addr6, sizeof(conn_addr6)) < 0)
			{
				JNX_LOGC("Error binding IPV6 socket - %s\n",strerror(errno));
				return -1;
			}
			break;
		default:
			bzero((char*)&conn_addr,sizeof(conn_addr));
			conn_addr.sin_addr.s_addr = INADDR_ANY;
			conn_addr.sin_family = s->addrfamily;
			conn_addr.sin_port = htons(port);
			if(bind(s->socket,(struct sockaddr*) &conn_addr, sizeof(conn_addr)) < 0)
			{
				JNX_LOGC("Error binding IPV4 socket - %s\n",strerror(errno));
				return -1;
			}
			break;
	}

	//IPV4
	struct sockaddr_in cli_addr;
	///IPV6
	struct sockaddr_in6 cli_addr6;

	int clilen = 0;
	int clisock;
	char buffer[MAXBUFFER];
	switch(s->stype)
	{
		case SOCK_STREAM:
			listen(s->socket,max_connections);
			int n = -1;
			while(1)
			{
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
					bzero(buffer,MAXBUFFER);
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
			break;
		case SOCK_DGRAM:	

			while(1)
			{
				switch(s->addrfamily)
				{
					case AF_INET6:
						bzero(buffer,MAXBUFFER);
						clilen = sizeof(cli_addr6);
						n = recvfrom(s->socket, buffer,MAXBUFFER,0,(struct sockaddr*)&cli_addr6,(socklen_t*)&clilen);
						printf("Printing from DGRAM IPV6 - %s\n",buffer);
						break;
					case AF_INET:
						bzero(buffer,MAXBUFFER);
						clilen = sizeof(cli_addr);	
						n = recvfrom(s->socket, buffer,MAXBUFFER,0,(struct sockaddr*)&cli_addr,(socklen_t*)&clilen);
						printf("Printing from DGRAM IPV4 - %s\n",buffer);
						break;
				}		
			}
			break;
		default:
			JNX_LOGC("Unsupported transport protocol\n");
			return -1;
	}
	return 0;
}

size_t jnx_network_broadcast(jnx_socket *s,ssize_t port, char *group, char *msg,ssize_t msg_len)
{
	if(!s->isclosed)
	{
		jnx_network_socket_close(s);
	}
	int rv;
	struct addrinfo hints, *servinfo, *p;
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	char buffer[128];
	sprintf(buffer,"%zu",port);
	if((rv = getaddrinfo(group,buffer,&hints,&servinfo)) != 0){
		JNX_LOGC("Error with getaddrinfo - %s\n",strerror(errno));
		return -1;
	}
	for(p = servinfo; p != NULL; p = p->ai_next){
		if((s->socket= socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1)
		{
			JNX_LOGC("Socket error - %s\n",strerror(errno));
			continue;
		}
		break;
	}
	int broadcast = 1;
	if(setsockopt(s->socket,SOL_SOCKET,SO_BROADCAST,&broadcast, sizeof broadcast) == -1)
	{
		JNX_LOGC("Broadcast could not set sock opts\n");
		return -1;;
	}

	freeaddrinfo(servinfo);
	if((sendto(s->socket,msg,msg_len,0,p->ai_addr,p->ai_addrlen)) == -1)
	{
		JNX_LOGC("Error on broadcast\n");
		return -1;
	}
	return 0;
}
size_t jnx_network_broadcast_listen(jnx_socket *s, ssize_t port, char *group, broadcast_listen_callback c)
{
	if(!s->isclosed)
	{
		jnx_network_socket_close(s);
	}
	struct addrinfo hints, *servinfo, *p;
	size_t nbytes;
	int rv;
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	char buffer[128];
	sprintf(buffer,"%zu",port);
	if((rv = getaddrinfo(group,buffer,&hints,&servinfo)) != 0){
		JNX_LOGC("Error with getaddrinfo - %s",strerror(errno));
		return -1;
	}
	for(p = servinfo; p != NULL; p = p->ai_next){
		if((s->socket= socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1)
		{
			JNX_LOGC("Socket error - %s",strerror(errno));
			continue;
		}
		break;
	}
	if(bind(s->socket, p->ai_addr,p->ai_addrlen) == -1)
	{
		JNX_LOGC("Error with binding broadcast listener - %s",strerror(errno));
		return -1;
	}	

	freeaddrinfo(servinfo);

	struct sockaddr_storage their_addr;
	socklen_t addr_len = sizeof their_addr;
	char obuffer[MAXBUFFER];

	while(1)
	{
		bzero(obuffer,MAXBUFFER);
		if((nbytes = recvfrom(s->socket,obuffer,MAXBUFFER,0,
						(struct sockaddr *)&their_addr,&addr_len)) == -1)
		{
			JNX_LOGC("Error with recvfrom - %s",strerror(errno));
			return -1;
		}	
		c(strdup(obuffer),nbytes);
	}

	return 0;
}



