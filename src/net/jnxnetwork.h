/*
 * =====================================================================================
 *
 *       Filename:  jnxnetwork.h
 *
 *    Description:  Networking API 
 *
 *        Version:  1.0
 *        Created:  02/20/14 12:50:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
/** @file jnxnetwork.h
 *  @brief The API for jnxlibc networking (includes TCP, UDP )
 *
 *  #include <jnxc_headers/jnxnetwork.h>
 */
#ifndef __JNXNETWORK_H__
#define __JNXNETWORK_H__
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#ifdef __cplusplus
	extern "C" {
#endif

typedef enum { IPV4 , IPV6 } jnx_address_family;

typedef struct 
{
	int socket;
	unsigned int addrfamily;
	ssize_t stype;
} jnx_socket;

typedef void (*listen_callback)(char *, size_t ,char *);
typedef void (*broadcast_listen_callback)(char *, size_t);

jnx_socket *jnx_network_socket_create(unsigned int addrfamily,ssize_t stype);

void jnx_network_socket_destroy(jnx_socket *s);

size_t jnx_network_send(jnx_socket *s, char *host, ssize_t port, char *msg, ssize_t msg_len);

size_t jnx_network_listen(jnx_socket *s, ssize_t port, ssize_t max_connections, listen_callback c);

size_t jnx_network_broadcast(jnx_socket *s, ssize_t port, char *group, char *msg,ssize_t msg_len);

size_t jnx_network_broadcast_listen(jnx_socket *s, ssize_t port, char *group, broadcast_listen_callback c);

#ifdef __cplusplus
	}
#endif
#endif
