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
//#include <assert.h>
//#include <netdb.h>
#include <stdio.h>
#include <string.h>
//#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
//#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <ifaddrs.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
//#include "jnxlog.h"
#include "jnxunixsocket.h"

//#define MAXBUFFER 1024
jnx_unix_socket *create_unix_socket(ssize_t stype, char *socket_path) {
	jnx_unix_socket *jus= calloc(1, sizeof(jnx_unix_socket));
	jus->isclosed = 0;
	int sock = socket(AF_UNIX, stype, 0);
	if (sock == -1) {
		return NULL;
	}
	jus->socket = sock;
	(jus->address).sun_family = AF_UNIX;
	strncpy((jus->address).sun_path, socket_path, sizeof((jus->address).sun_path) - 1);
	jus->stype = stype;
	return jus;
}
jnx_unix_socket *jnx_unix_socket_stream_create(char *socket_path) {
	return create_unix_socket(SOCK_STREAM, socket_path);
}
jnx_unix_socket *jnx_unix_socket_datagram_create(char *socket_path) {
	return create_unix_socket(SOCK_DGRAM, socket_path);
}
void jnx_unix_socket_close(jnx_unix_socket *s) {
	if (!s->isclosed) {
		close(s->socket);
		remove((s->address).sun_path);
		s->isclosed = 1;
	}
}
void jnx_unix_socket_destroy(jnx_unix_socket **s) {
	jnx_unix_socket_close(*s);
	free(*s);
	*s = NULL;
}
ssize_t write_to_stream_socket(jnx_unix_socket *s, char *msg, ssize_t msg_len) {
	size_t tbytes = 0;
	size_t rbytes = msg_len;

	while(tbytes < rbytes) {
		size_t n = write(s->socket,msg,rbytes);
		if(n == -1) {
			perror("send");
			return 0;
		}
		tbytes +=n;
		rbytes = msg_len - tbytes;
	}
	return tbytes;
}
ssize_t jnx_unix_socket_stream_send(jnx_unix_socket *s, char *msg, ssize_t msg_len) {
	if (connect(s->socket,(struct sockaddr *)&(s->address),sizeof(struct sockaddr_un)) == -1) {
		perror("connect");
		return 0;
	}

	return write_to_stream_socket(s, msg, msg_len);
}
ssize_t jnx_unix_socket_datagram_send(jnx_unix_socket *s, char *msg, ssize_t msg_len) {
	size_t tbytes = 0;
	size_t rbytes = msg_len;

	while(tbytes < rbytes) {
		size_t n = sendto(s->socket,msg,msg_len,0,(struct sockaddr *)&(s->address),sizeof(struct sockaddr_un));
		if(n == -1) {
			perror("sendto");
			return 0;
		}
		tbytes +=n;
		rbytes = msg_len - tbytes;
	}
	return tbytes;
}
//int jnx_socket_tcp_listen(jnx_socket *s, char* port, ssize_t max_connections, tcp_socket_listener_callback c) {
//	assert(s);
//	assert(port);
//	assert(s->isclosed == 0);
//	assert(s->stype == SOCK_STREAM);
//	int optval = 1;
//	struct addrinfo hints, *res, *p;
//	struct sockaddr_storage their_addr;
//	char buffer[MAXBUFFER];
//	memset(&hints,0,sizeof(hints));
//	hints.ai_family = s->addrfamily;
//	hints.ai_socktype = s->stype;
//
//	getaddrinfo(NULL,port,&hints,&res);
//
//	p = res;
//	while(p != NULL) {
//		if((s->socket = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1) {
//			perror("server: socket");
//			return -1;
//		}
//		if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &optval,
//					sizeof(int)) == -1) {
//			perror("setsockopt");
//			exit(1);
//		}
//		if (bind(s->socket, p->ai_addr, p->ai_addrlen) == -1) {
//			perror("server: bind");
//			return -1;
//		}
//		break;
//
//		p = p->ai_next;
//	}
//	freeaddrinfo(res);
//	listen(s->socket,max_connections);
//	while(1) {
//		socklen_t addr_size = sizeof(their_addr);
//		int recfd = accept(s->socket,(struct sockaddr*)&their_addr,&addr_size);
//		if(recfd < 0) {
//			JNX_LOGC(JLOG_ALERT,"accept: %s",strerror(errno));
//			return -1;
//		}
//		bzero(buffer,MAXBUFFER);
//		FILE *fp = tmpfile();
//		size_t bytesread = read(recfd,buffer,MAXBUFFER);
//		fwrite(buffer,sizeof(char),bytesread,fp);
//
//		while(bytesread > 0) {
//			bzero(buffer,MAXBUFFER);
//			bytesread = read(recfd,buffer,MAXBUFFER);
//			if(bytesread == -1) {
//				perror("read:");
//				fclose(fp);
//				return -1;
//			}
//			if(bytesread > 0) {
//				fwrite(buffer,sizeof(char),bytesread,fp);
//			}
//		}
//		int len = ftell(fp);
//		rewind(fp);
//		char *out = calloc(len + 1, sizeof(char));
//		fread(out,sizeof(char),len,fp);
//		fclose(fp);
//
//		int ret = 0;
//		if((ret = c(out,len,jnx_socket_tcp_resolve_ipaddress(recfd))) != 0) {
//			printf("Exiting tcp listener with %d\n",ret);
//			return 0;
//		}
//	}
//	return 0;
//}
int jnx_unix_socket_stream_listen(jnx_unix_socket *s, ssize_t max_connections, stream_socket_listener_callback c) {
	return 0;
}
//int jnx_socket_udp_listen(jnx_socket *s, char* port, ssize_t max_connections, udp_socket_listener_callback c) {
//	assert(s);
//	assert(port);
//	assert(s->isclosed == 0);
//	assert(s->stype == SOCK_DGRAM);
//	int optval = 1;
//	struct addrinfo hints, *res, *p;
//	struct sockaddr_storage their_addr;
//	socklen_t their_len = sizeof(their_addr);
//	char buffer[MAXBUFFER];
//	memset(&hints,0,sizeof(hints));
//	hints.ai_family = s->addrfamily;
//	hints.ai_socktype = s->stype;
//	hints.ai_flags = AI_PASSIVE;
//
//	getaddrinfo(NULL,port,&hints,&res);
//
//	p = res;
//	while(p != NULL) {
//		if((s->socket = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1) {
//			perror("server: socket");
//			return -1;
//		}
//		if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &optval,
//					sizeof(int)) == -1) {
//			perror("setsockopt");
//			exit(1);
//		}
//		if (bind(s->socket, p->ai_addr, p->ai_addrlen) == -1) {
//			perror("server: bind");
//			return -1;
//		}
//		break;
//		p= p->ai_next;
//	}
//	freeaddrinfo(res);
//
//	while(1) {
//		bzero(buffer,MAXBUFFER);
//		size_t bytesread = recvfrom(s->socket,buffer,MAXBUFFER,0,(struct sockaddr *)&their_addr,(socklen_t*)&their_len);
//
//		if(bytesread == -1) {
//			perror("recvcfrom:");
//			return -1;
//		}
//		int ret = 0;
//		char *outbuffer = malloc(bytesread * sizeof(char));
//		memcpy(outbuffer,buffer,bytesread);
//		if((ret = c(outbuffer,bytesread,jnx_socket_udp_resolve_ipaddress(their_addr))) != 0) {
//			printf("Exiting udp listener with %d",ret);
//			return 0;
//		}
//	}
//	return -1;
//}
int jnx_unix_socket_datagram_listen(jnx_unix_socket *s, ssize_t max_connections, datagram_socket_listener_callback c) {
	return 0;
}
