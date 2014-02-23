/*
 * =====================================================================================
 *
 *       Filename:  jnxsocket.h
 *
 *    Description:  Low level network API 
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
/** @file jnxsocket.h
 *  @brief The API for jnxlibc networking (includes TCP, UDP )
 *
 *  #include <jnxc_headers/jnxsocket.h>
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

typedef void (*listen_callback)(char *, size_t ,jnx_socket *);
/**
 * @fn jnx_socket *jnx_socket_tcp_create(unsigned int addrfamily)
 * @brief creates a jnx tcp socket
 * @param addrfamily this is the address family to use (e.g. AF_UNIX)
 * @return jnx_socket
 */
jnx_socket *jnx_socket_tcp_create(unsigned int addrfamily);
/**
 * @fn jnx_socket *jnx_socket_udp_create(unsigned int addrfamily)
 * @brief creates a jnx udp socket
 * @param addrfamily this is the address family to use (e.g. AF_UNIX)
 * @return jnx_socket
 */
jnx_socket *jnx_socket_udp_create(unsigned int addrfamily);
/** 
 *@fn jnx_socket_socket_close(jnx_socket *s)
 *@brief close the socket but maintain the socket object
 *@params s the socket object to close
 */
void jnx_socket_close(jnx_socket *s);
/**
 * @fn void jnx_socket_socket_destroy(jnx_socket *s)
 * @param s is the socket to destroy
 */
void jnx_socket_destroy(jnx_socket **s);
/**
 * @fn jnx_socket_tcp_send((jnx_socket *s, char *host, char* port, char *msg, ssize_t msg_len)
 * @param s is the socket to use to send
 * @param host is the target destination
 * @param port is the target port
 * @param msg is the payload to send
 * @param msg_len is the size of payload
 * @return size_t of bytes sent, -1 for failure
 */
size_t jnx_socket_tcp_send(jnx_socket *s, char *host, char* port, char *msg, ssize_t msg_len);
/**
 * @fn jnx_socket_udp_send(jnx_socket *s, char *host, char* port, char *msg, ssize_t msg_len)
 * @param s is the socket to use to send
 * @param host is the target destination
 * @param port is the target port
 * @param msg is the payload to send
 * @param msg_len is the size of payload
 * @return size_t of bytes sent, -1 for failure
 */
size_t jnx_socket_udp_send(jnx_socket *s, char *host, char* port, char *msg, ssize_t msg_len);
/**
 * @fn size_t jnx_socket_tpc_listen(jnx_socket *s, char* port, ssize_t max_connections, listen_callback c)
 * @param s is the socket to use to send
 * @param port is the target port
 * @param max_connections are the number of connetions in the queue
 * @param c is the function pointer callback for received messages
 * @return -1 on error
 */
size_t jnx_socket_tcp_listen(jnx_socket *s, char* port, ssize_t max_connections, listen_callback c);
/**
 * @fn size_t jnx_socket_udp_listen(jnx_socket *s, char* port, ssize_t max_connections, listen_callback c)
 * @param s is the socket to use to send
 * @param port is the target port
 * @param max_connections are the number of connetions in the queue
 * @param c is the function pointer callback for received messages
 * @return -1 on error
 */
size_t jnx_socket_udp_listen(jnx_socket *s, char* port, ssize_t max_connections, listen_callback c);
#ifdef __cplusplus
	}
#endif
#endif
