#ifndef __JNXNETWORK_H__
#define __JNXNETWORK_H__
#include <stddef.h>
/** @file jnxnetwork.h
 *  @brief The API for jnxlibc networking (includes TCP, UDP )
 *
 *  #include <jnxc_headers/jnxnetwork.h>
 */

/**
 * @brief Maximum buffer size allowed 
 */
#define MAXBUFFER 4028

/*-----------------------------------------------------------------------------
 * Currently the library defaults to the use of IPv4, this can be overriden with USEIPV6 
 *-----------------------------------------------------------------------------*/
#ifndef JNXNETWORK_USEIPV6
#define ADDRESSFAMILY AF_INET
#else
#define ADDRESSFAMILY AF_INET6
#endif
/**
 *  @brief This is the callback for the received buffer from the network listener
 *  @warning User must free the msg as it has been malloced for fread purposes
 *  @warning User does not need to free client_ip_addr
 */
typedef void (*jnx_network_listener_callback)(char* msg,size_t bytes,char*client_ip_addr);

typedef void (*jnx_network_broadcast_listener_callback)(char* msg);

/** @fn jnx_network_setup_listener(int port, int max_connections, void (*Callback)(char*,size_t,char*))
 *  @brief Using port and a function pointer callback it will create a listener
 *  @param port selet target listener port
 *  @param max_connections is the size of the backlog queue
 *  @param Callback this is a function pointer to your jnx_network_listener_callback
 *  @return status code 0 success
 */
int jnx_network_setup_listener(int port,int max_connections, void (*Callback)(char*,size_t,char*));

/** @fn jnx_network_cancel_listener(void)
 * @brief Cancels the last set listener
 */
void jnx_network_cancel_listener(void);

/** @fn jnx_network_send_message(char* host, int port, char* msg, size_t msg_len)
 * @brief This sends a message via selected port to host
 *
 * @param host the ip address of the host you wish to connect to
 * @param port port number to connect to
 * @param msg message to be sent
 * @param msg_len length of message
 * @return returns number of bytes sent success, -1 on failure
 */
int jnx_network_send_message(char* host, int port, char* msg, size_t msg_len);

/** @fn jnx_network_send_broadcast(int port, char *broadcastgroup,char *message)
 * @brief Sends a UDP datagram across the selected group range 
 *
 * @param port select the port for transmission
 * @param broadcastgroup this group can be set to these standards 
 * http://www.tldp.org/HOWTO/Multicast-HOWTO-2.html
 * @param message the message to transmit to the broadcast group
 */
int jnx_network_send_broadcast(int port,char *broadcastgroup,char *message);

/**@fn jnx_network_broadcast_listener(int port, char *broadcastgroup, void(*callback_function)(char*))
 * @brief sets up a listener for udp datagram broadcasts
 *
 * @param port target port to listen on
 * @param broadcastgroup target group to register to
 * @param callback_function pointer to the callback with received buffer
 */
void jnx_network_broadcast_listener(int port,char *broadcastgroup, void(*callback_function)(char*));

/** @fn jnx_network_local_ip(char *interface) 
 * @brief finds the current machine ip address for given interface
 *
 * @param interface the target interface e.g. 'wlan0'
 *
 * @return a char* of ip address or NULL if not found
 */
char* jnx_network_local_ip(char* interface);
#endif
