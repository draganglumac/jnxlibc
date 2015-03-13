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
/** @file jnxunixsocket.h
 *  @brief The API for jnxlibc inter-process communication using Unix domain sockets.
 *
 *  #include <jnxc_headers/jnxunixsocket.h>
 */
#ifndef __JNXUNIXSOCKET_H__
#define __JNXUNIXSOCKET_H__
#include "jnxtypes.h"
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
    jnx_uint32 isclosed;
    jnx_uint32 islisten;
    jnx_int32 socket;
    jnx_unix_socket_address address;
    jnx_size stype;
  } jnx_unix_socket;

  /*
   *@warning must return 0 or will break the listener loop
   */
  typedef jnx_int32 (*stream_socket_listener_callback_with_context)(jnx_uint8 *payload, jnx_size bytesread, jnx_unix_socket *remote_sock, void *context);
  typedef jnx_int32 (*stream_socket_listener_callback)(jnx_uint8 *payload, jnx_size bytesread, jnx_unix_socket *remote_sock);
  /*
   *@warning must return 0 or will break the listener loop
   */
  typedef jnx_int32 (*datagram_socket_listener_callback_with_context)(jnx_uint8 *payload, jnx_size bytesread, jnx_unix_socket *remote_sock, void *context);
  typedef jnx_int32 (*datagram_socket_listener_callback)(jnx_uint8 *payload, jnx_size bytesread, jnx_unix_socket *remote_sock);
  /**
   * @fn jnx_unix_socket *jnx_unix_stream_socket_create(unsigned jnx_uint32 addrfamily)
   * @brief creates a jnx stream socket
   * @param socket_path path to the socket file on the file system
   * @return jnx_unix_socket
   */
  jnx_unix_socket *jnx_unix_stream_socket_create(jnx_char *socket_path);
  /**
   * @fn jnx_unix_socket *jnx_unix_datagram_socket_create(unsigned jnx_uint32 addrfamily)
   * @brief creates a jnx datagram socket
   * @param socket_path path to the socket file on the file system
   * @return jnx_unix_socket
   */
  jnx_unix_socket *jnx_unix_datagram_socket_create(jnx_char *socket_path);
  /**
   *@fn jnx_unix_socket_socket_close(jnx_unix_socket *s)
   *@brief close the socket but majnx_uint32ain the socket object
   *@params s the socket object to close
   */
  void jnx_unix_socket_close(jnx_unix_socket *s);
  /**
   * @fn void jnx_unix_socket_socket_destroy(jnx_unix_socket *s)
   * @param s is the socket to destroy
   */
  void jnx_unix_socket_destroy(jnx_unix_socket **s);
  /**
   * @fn jnx_unix_stream_socket_send((jnx_unix_socket *s,, jnx_uint8*msg, jnx_size msg_len)
   * @param s is the socket to use to send
   * @param msg is the payload to send
   * @param msg_len is the size of payload
   * @return jnx_size of bytes sent
   */
  jnx_size jnx_unix_stream_socket_send(jnx_unix_socket *s, jnx_uint8 *msg, jnx_size msg_len);
  /**
   * @fn jnx_unix_datagram_socket_send(jnx_unix_socket *s, jnx_uint8*msg, jnx_size msg_len)
   * @param s is the socket to use to send
   * @param msg is the payload to send
   * @param msg_len is the size of payload
   * @return jnx_size of bytes sent
   */
  jnx_size jnx_unix_datagram_socket_send(jnx_unix_socket *s, jnx_uint8*msg, jnx_size msg_len);
  /**
   * @fn jnx_uint32 jnx_unix_stream_socket_listen(jnx_unix_socket *s,  jnx_size max_connections, stream_socket_listener_callback c)
   * @param max_connections are the number of connetions in the queue
   * @param c is the function pointer callback for received messages
   * @param context is context that gets passed to the callback
   * @return -1 on error
   */
  jnx_int32 jnx_unix_stream_socket_listen_with_context(jnx_unix_socket *s, jnx_size max_connections, stream_socket_listener_callback_with_context c, void *context);
  /**
   * @fn jnx_uint32 jnx_unix_stream_socket_listen(jnx_unix_socket *s,  jnx_size max_connections, stream_socket_listener_callback c)
   * @param max_connections are the number of connetions in the queue
   * @param c is the function pointer callback for received messages
   * @return -1 on error
   */
  jnx_int32 jnx_unix_stream_socket_listen(jnx_unix_socket *s, jnx_size max_connections, stream_socket_listener_callback c);
  /**
   * @fn jnx_uint32 jnx_unix_datagram_socket_listen(jnx_unix_socket *s, jnx_size max_connections, datagram_socket_listener_callback c)
   * @param s is the socket to use to send
   * @param c is the function pojnx_uint32er callback for received messages
   * @param context is context that gets passed to the callback
   * @return -1 on error
   */
  jnx_int32 jnx_unix_datagram_socket_listen_with_context(jnx_unix_socket *s, datagram_socket_listener_callback_with_context c, void *context);
  /**
   * @fn jnx_uint32 jnx_unix_datagram_socket_listen(jnx_unix_socket *s, jnx_size max_connections, datagram_socket_listener_callback c)
   * @param s is the socket to use to send
   * @param c is the function pojnx_uint32er callback for received messages
   * @return -1 on error
   */
  jnx_int32 jnx_unix_datagram_socket_listen(jnx_unix_socket *s, datagram_socket_listener_callback c);
#ifdef __cplusplus
}
#endif
#endif
