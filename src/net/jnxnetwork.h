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

typedef struct 
{
	int isclosed;
	int socket;
	unsigned int addrfamily;
	ssize_t stype;
} jnx_socket;

typedef void (*listen_callback)(char *, size_t ,char *);
typedef void (*broadcast_listen_callback)(char *, size_t);

/**
 * @fn jnx_socket *jnx_network_socket_create(unsigned int addrfamily,ssize_t stype)
 * @brief creates a jnx socket
 * @param addrfamily this is the address family to use (e.g. AF_UNIX)
 * @param stypes this is the socket type to use (e.g. SOCK_STREAM)
 * @return jnx_socket
 */
jnx_socket *jnx_network_socket_create(unsigned int addrfamily,ssize_t stype);

/** 
 *@fn jnx_network_socket_close(jnx_socket *s)
 *@brief close the socket but maintain the socket object
 *@params s the socket object to close
 */
void jnx_network_socket_close(jnx_socket *s);
/**
 * @fn void jnx_network_socket_destroy(jnx_socket *s)
 * @param s is the socket to destroy
 */
void jnx_network_socket_destroy(jnx_socket *s);
/**
 * @fn jnx_network_send(jnx_socket *s, char *host, ssize_t port, char *msg, ssize_t msg_len)
 * @param s is the socket to use to send
 * @param host is the target destination
 * @param port is the target port
 * @param msg is the payload to send
 * @param msg_len is the size of payload
 * @return size_t of bytes sent, -1 for failure
 */
size_t jnx_network_send(jnx_socket *s, char *host, ssize_t port, char *msg, ssize_t msg_len);
/**
 * @fn size_t jnx_network_listen(jnx_socket *s, ssize_t port, ssize_t max_connections, listen_callback c)
 * @param s is the socket to use to send
 * @param port is the target port
 * @param max_connections are the number of connetions in the queue
 * @param c is the function pointer callback for received messages
 * @return -1 on error
 */
size_t jnx_network_listen(jnx_socket *s, ssize_t port, ssize_t max_connections, listen_callback c);
/**
 * @fn size_t jnx_network_broadcast(jnx_socket *S, ssize_t port, char *group, char *msg, ssize_t msg_len)
 * @warning not supported by IPV6
 * @param s is the socket to use to send
 * @param port is the target port
 * @param group is the broadcast group (e.g. 255.0.0.25)
 * @param msg is the payload to send
 * @param msg_len is the size of payload
 * @return -1 on error 
 */
size_t jnx_network_broadcast(jnx_socket *s, ssize_t port, char *group, char *msg,ssize_t msg_len);
/**
 * @fn size_t jnx_network_broadcast_listen(jnx_socket *s, ssize_t port, char *group, broadcast_callback c)
 * @warning not supported by IPV6
 * @param s is the socket to use to send
 * @param port is the target port
 * @param group is the broadcast group (e.g. 255.0.0.25)
 * @param c is the function pointer callback for received messages
 * @return -1 on error
 */
size_t jnx_network_broadcast_listen(jnx_socket *s, ssize_t port, char *group, broadcast_listen_callback c);

#ifdef __cplusplus
	}
#endif
#endif
