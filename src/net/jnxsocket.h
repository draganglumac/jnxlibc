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
#include "jnxtypes.h"
#include <stddef.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#ifdef __cplusplus
extern "C" {
#endif
  typedef struct
  {
    jnx_int32 isclosed;
    jnx_int32 isconnected;
    jnx_int32 socket;
    jnx_unsigned_int addrfamily;
    jnx_size stype;
    jnx_char *ipaddress;
  } jnx_socket;

  /*
   *@warning must return 0 or will break the listener loop
   */
  typedef jnx_int32 (*tcp_socket_listener_callback)(jnx_uint8 *payload, jnx_size bytesread, jnx_socket *s);
  typedef jnx_int32 (*tcp_socket_listener_callback_with_context)(jnx_uint8 *payload, jnx_size bytesread, jnx_socket *s, void *context);
  /*
   *@warning must return 0 or will break the listener loop
   */
  typedef jnx_int32 (*udp_socket_listener_callback)(jnx_uint8 *payload, jnx_size bytesread, jnx_socket *s);
  typedef jnx_int32 (*udp_socket_listener_callback_with_context)(jnx_uint8 *payload, jnx_size bytesread, jnx_socket *s, void *context);
  /**
   * @fn jnx_socket *jnx_socket_tcp_create(jnx_unsigned_int addrfamily)
   * @brief creates a jnx tcp socket
   * @param addrfamily this is the address family to use (e.g. AF_INET)
   * @return jnx_socket
   */
  jnx_socket *jnx_socket_tcp_create(jnx_unsigned_int addrfamily);
  /**
   * @fn jnx_socket *jnx_socket_udp_create(jnx_unsigned_int addrfamily)
   * @brief creates a jnx udp socket
   * @param addrfamily this is the address family to use (e.g. AF_INET)
   * @return jnx_socket
   */
  jnx_socket *jnx_socket_udp_create(jnx_unsigned_int addrfamily);
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
   *@fn jnx_size jnx_socket_udp_enable_broadcast(socket *s)
   *@brief this IPV4 option will enable udp broadcasting
   *@param s is the socket object to enable broadcasting
   *@return 0 on success, -1 on error
   */
  jnx_size jnx_socket_udp_enable_broadcast_send_or_listen(jnx_socket *s);
  /**
   *@fn jnx_size jnx_socket_udp_enable_multicast_send(jnx_socket *s, jnx_char *interface, jnx_int32 ignore_local)
   *@brief This is function will enable the given socket to transmit multicast packets over domain
   *@param s is the socket to enable multicast sending on
   *@param interface is the IP address of the interface to use on the local machine
   *@param ignore_local is a flag to either disable local multicast loopback
   *@param returns 0 on success
   */
  jnx_size jnx_socket_udp_enable_multicast_send(jnx_socket *s, jnx_char *interface, jnx_int32 ignore_local);
  /**
   *@fn jnx_size jnx_socket_udp_enable_multicast_listen(jnx_socket *s, jnx_char *interface,jnx_char *group)
   *@brief This function will enable the given socket to recieve multicast packets on the given interface for the subscribed group
   *@param s is the socket to enable multicast listening on
   *@param interface is the IP address of the interface to use on the local machine
   *@param group is the multicast group to subscribe too
   *@return returns 0 on success
   */
  jnx_size jnx_socket_udp_enable_multicast_listen(jnx_socket *s, jnx_char *interface, jnx_char *group);
  /**
   * @fn jnx_socket_tcp_send((jnx_socket *s, jnx_char *host, jnx_char* port, jnx_uint8 *msg, jnx_size msg_len)
   * @param s is the socket to use to send
   * @param host is the target destination
   * @param port is the target port
   * @param msg is the payload to send
   * @param msg_len is the size of payload
   * @return jnx_size of bytes sent
   */
  jnx_size jnx_socket_tcp_send(jnx_socket *s, jnx_char *host, jnx_char* port, jnx_uint8 *msg, jnx_size msg_len);
  /**
   * @fn jnx_socket_tcp_send_with_receipt(jnx_socket *s, jnx_char *host, jnx_char* port, jnx_uint8 *msg, jnx_size msg_len,jnx_uint8 **out_receipt)
   * @param s is the socket to use to send
   * @param host is the target destination
   * @param port is the target port
   * @param msg is the payload to send
   * @param msg_len is the size of payload
   * @param out_receipt returned from the target host
   * @return jnx_size of bytes sent
   */
  jnx_size jnx_socket_tcp_send_with_receipt(jnx_socket *s, jnx_char *host, jnx_char* port, jnx_uint8 *msg, jnx_size msg_len,jnx_uint8 **out_receipt);
  /**
   * @fn jnx_socket_udp_send(jnx_socket *s, jnx_char *host, jnx_char* port, jnx_uint8 *msg, jnx_size msg_len)
   * @param s is the socket to use to send
   * @param host is the target destination
   * @param port is the target port
   * @param msg is the payload to send
   * @param msg_len is the size of payload
   * @warning UDP header is 16bit which means 65k is the max file transmission size
   * @return jnx_size of bytes sent
   */
  jnx_size jnx_socket_udp_send(jnx_socket *s, jnx_char *host, jnx_char* port, jnx_uint8 *msg, jnx_size msg_len);
  /**
   * @fn jnx_int32 jnx_socket_tpc_listen(jnx_socket *s, jnx_char* port, jnx_size max_connections, socket_listener_callback c)
   * @param s is the socket to use to send
   * @param port is the target port
   * @param max_connections are the number of connetions in the queue
   * @param c is the function pointer callback for received messages
   * @return -1 on error
   */
  int jnx_socket_tcp_listen(jnx_socket *s, jnx_char* port, jnx_size max_connections, tcp_socket_listener_callback c);
  /**
   * @fn jnx_int32 jnx_socket_udp_listen(jnx_socket *s, jnx_char* port, jnx_size max_connections, socket_listener_callback c)
   * @param s is the socket to use to send
   * @param port is the target port
   * @param max_connections are the number of connetions in the queue
   * @param c is the function pointer callback for received messages
   * @return -1 on error
   */
  int jnx_socket_udp_listen(jnx_socket *s, jnx_char* port, jnx_size max_connections, udp_socket_listener_callback c);
  /**
   * @fn jnx_int32 jnx_socket_tpc_listen(jnx_socket *s, jnx_char* port, jnx_size max_connections, socket_listener_callback c)
   * @param s is the socket to use to send
   * @param port is the target port
   * @param max_connections are the number of connetions in the queue
   * @param c is the function pointer callback for received messages
   * @param context is the data to pass to the callback as contextual information
   * @return -1 on error
   */
  int jnx_socket_tcp_listen_with_context(jnx_socket *s, jnx_char* port, jnx_size max_connections, tcp_socket_listener_callback_with_context c, void *context);
  /**
   * @fn jnx_int32 jnx_socket_udp_listen(jnx_socket *s, jnx_char* port, jnx_size max_connections, socket_listener_callback c)
   * @param s is the socket to use to send
   * @param port is the target port
   * @param max_connections are the number of connetions in the queue
   * @param c is the function pointer callback for received messages
   * @param context is the data to pass to the callback as contextual information
   * @return -1 on error
   */
  int jnx_socket_udp_listen_with_context(jnx_socket *s, jnx_char* port, jnx_size max_connections, udp_socket_listener_callback_with_context c, void *context);

#ifdef __cplusplus
}
#endif
#endif
