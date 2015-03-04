/*
 * jnx_udp_socket.h
 * Copyright (C) 2015 tibbar <tibbar@debian>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef JNX_UDP_SOCKET_H
#define JNX_UDP_SOCKET_H
#include "jnxsocket.h"
#ifdef __cplusplus
extern "C" {
#endif

  typedef struct jnx_udp_listener {
    jnx_socket *socket;
  }jnx_udp_listener;

  typedef void (*jnx_udp_listener_callback)(const jnx_uint8 *payload, \
      jnx_size bytes_read, void *args);

  /**
   * @fn jnx_socket *jnx_socket_udp_create(jnx_unsigned_int addrfamily)
   * @brief creates a jnx udp socket
   * @param addrfamily this is the address family to use (e.g. AF_INET)
   * @return jnx_socket
   */
  jnx_socket *jnx_socket_udp_create(jnx_unsigned_int addrfamily);
  /**
   *@fn jnx_udp_listener *jnx_socket_udp_listener_create(char *port,
   *jnx_unsigned_int family, int max_connections)
   *@brief Creates a listener that can be ticked and retrieve incoming network traffic
   *@param family is the address family to use (AF_INET/AF_INET6)
   *@param max connections is the maximum backlog size on the listener
   *@return pointer to a listener on success
   */
  jnx_udp_listener* jnx_socket_udp_listener_create(jnx_char *port,
      jnx_unsigned_int family, jnx_int max_connections);
  /**
   *@fn void jnx_socket_udp_listener_destroy(jnx_udp_listener **listener)
   *@brief destroys and closes socket for the udp listener
   *@param listener is a pointer to pointer of the current listener
   */
  void jnx_socket_udp_listener_destroy(jnx_udp_listener **listener);
  /**
   *@fn jnx_socket_udp_listener_tick(jnx_udp_listener *listener,
   * jnx_udp_listener_callback, void *args)
   *@brief tick will need to be called manually by the user to accept and recv
   *incoming network data. It has been designed for use in a loop 
   *and to provide max user control
   *@warning NONBLOCKING
   *@param listener is the current instantiated listener
   *@param callback is passed into the tick representing the function
   *to return received data too
   *@param args are the context arguments to pass to the receiver function 
   *can be null
   */
  void jnx_socket_udp_listener_tick(jnx_udp_listener* listener,
      jnx_udp_listener_callback callback, void *args);
  /**
   *@fn jnx_socket_udp_listener_tick(jnx_udp_listener *listener,
   * jnx_udp_listener_callback, void *args)
   *@brief auto tick will block and loop automatically for the user
   *user to accept and recv incoming network data.
   *@warning BLOCKING
   *@param listener is the current instantiated listener
   *@param callback is passed into the tick representing the function
   *to return received data too
   *@param args are the context arguments to pass to the receiver function 
   *can be null
   */
  void jnx_socket_udp_listener_auto_tick(jnx_udp_listener *listener,
      jnx_udp_listener_callback callback, void *args);
  /**
   * @fn jnx_char *jnx_socket_udp_resolve_ipaddress(struct sockaddr_storage sa);
   * @brief if successful it will return a string displaying 
   * the readable IP address
   * @param sa is the sockaddr_stoage to pass through from the accept call
   * @return jnx_char string or NULL
   */   
  jnx_char *jnx_socket_udp_resolve_ipaddress(struct sockaddr_storage sa);
  /**
   *@fn jnx_size jnx_socket_udp_enable_broadcast(socket *s)
   *@brief this IPV4 option will enable udp broadcasting
   *@param s is the socket object to enable broadcasting
   *@return 0 on success, -1 on error
   */
  jnx_size jnx_socket_udp_enable_broadcast_send_or_listen(jnx_socket *s);
  /**
   *@fn jnx_size jnx_socket_udp_enable_multicast_send
   (jnx_socket *s, jnx_char *interface, jnx_int32 ignore_local)
   *@brief This is function will enable the given socket to 
   *transmit multicast packets over domain
   *@param s is the socket to enable multicast sending on
   *@param interface is the IP address of the interface 
   *to use on the local machine
   *@param ignore_local is a flag to either disable local multicast loopback
   *@param returns 0 on success
   */
  jnx_size jnx_socket_udp_enable_multicast_send(jnx_socket *s,\
      jnx_char *interface, jnx_int32 ignore_local);
  /**
   *@fn jnx_size jnx_socket_udp_enable_multicast_listen
   (jnx_socket *s, jnx_char *interface,jnx_char *group)
   *@brief This function will enable the given socket to 
   recieve multicast packets on the given interface for the subscribed group
   *@param s is the socket to enable multicast listening on
   *@param interface is the IP address of the interface 
   *to use on the local machine
   *@param group is the multicast group to subscribe too
   *@return returns 0 on success
   */
  jnx_size jnx_socket_udp_enable_multicast_listen(jnx_socket *s,\
      jnx_char *interface, jnx_char *group);
  /**
   * @fn jnx_socket_udp_send(jnx_socket *s, jnx_char *host,
   jnx_char* port, jnx_uint8 *msg, jnx_size msg_len)
   * @param s is the socket to use to send
   * @param host is the target destination
   * @param port is the target port
   * @param msg is the payload to send
   * @param msg_len is the size of payload
   * @warning UDP header is 16bit which means 65k is the
   * max file transmission size
   * @return jnx_size of bytes sent
   */
  jnx_size jnx_socket_udp_send(jnx_socket *s, jnx_char *host,\
      jnx_char* port, jnx_uint8 *msg, jnx_size msg_len);

#ifdef __cplusplus
}
#endif
#endif /* !JNX_UDP_SOCKET_H */
