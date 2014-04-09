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
#ifndef __JNXSOCKET_H__
#define __JNXSOCKET_H__
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef struct sockaddr_un jnx_unix_socket_address;

typedef struct
{
	int isclosed;
	int socket;
	jnx_unix_socket_address address
	ssize_t stype;
} jnx_unix_socket;

/*
 *@warning must return 0 or will break the listener loop
 */
typedef int (*stream_socket_listener_callback)(uint8_t *payload, size_t bytesread, char *ipaddress);
/*
 *@warning must return 0 or will break the listener loop
 */
typedef int (*datagram_socket_listener_callback)(uint8_t *payload, size_t bytesread, char *ipaddress);
/**
 * @fn jnx_unix_socket *jnx_unix_socket_stream_create(unsigned int addrfamily)
 * @brief creates a jnx stream socket
 * @param addrfamily this is the address family to use (e.g. AF_INET)
 * @param socket_path path to the socket file on the file system
 * @return jnx_unix_socket
 */
jnx_unix_socket *jnx_unix_socket_stream_create(unsigned int addrfamily, char *socket_path);
/**
 * @fn jnx_unix_socket *jnx_unix_socket_datagram_create(unsigned int addrfamily)
 * @brief creates a jnx datagram socket
 * @param addrfamily this is the address family to use (e.g. AF_INET)
 * @param socket_path path to the socket file on the file system
 * @return jnx_unix_socket
 */
jnx_unix_socket *jnx_unix_socket_datagram_create(unsigned int addrfamily, char *socket_path);
/**
 *@fn jnx_unix_socket_socket_close(jnx_unix_socket *s)
 *@brief close the socket but maintain the socket object
 *@params s the socket object to close
 */
void jnx_unix_socket_close(jnx_unix_socket *s);
/**
 * @fn void jnx_unix_socket_socket_destroy(jnx_unix_socket *s)
 * @param s is the socket to destroy
 */
void jnx_unix_socket_destroy(jnx_unix_socket **s);
/**
 * @fn jnx_unix_socket_stream_send((jnx_unix_socket *s, char *socket_path, uint8_t *msg, ssize_t msg_len)
 * @param s is the socket to use to send
 * @param msg is the payload to send
 * @param msg_len is the size of payload
 * @return size_t of bytes sent
 */
ssize_t jnx_unix_socket_stream_send(jnx_unix_socket *s, uint8_t *msg, ssize_t msg_len);
/**
 * @fn jnx_unix_socket_datagram_send(jnx_unix_socket *s, char *socket_path, uint8_t *msg, ssize_t msg_len)
 * @param s is the socket to use to send
 * @param msg is the payload to send
 * @param msg_len is the size of payload
 * @return size_t of bytes sent
 */
ssize_t jnx_unix_socket_datagram_send(jnx_unix_socket *s, uint8_t *msg, ssize_t msg_len);
/**
 * @fn int jnx_unix_socket_stream_listen(jnx_unix_socket *s, char *socket_path, ssize_t max_connections, stream_socket_listener_callback c)
 * @param max_connections are the number of connetions in the queue
 * @param c is the function pointer callback for received messages
 * @return -1 on error
 */
int jnx_unix_socket_stream_listen(jnx_unix_socket *s, ssize_t max_connections, stream_socket_listener_callback c);
/**
 * @fn int jnx_unix_socket_datagram_listen(jnx_unix_socket *s, char *socket_path, ssize_t max_connections, datagram_socket_listener_callback c)
 * @param s is the socket to use to send
 * @param max_connections are the number of connetions in the queue
 * @param c is the function pointer callback for received messages
 * @return -1 on error
 */
int jnx_unix_socket_datagram_listen(jnx_unix_socket *s, ssize_t max_connections, datagram_socket_listener_callback c);
#ifdef __cplusplus
}
#endif
#endif
