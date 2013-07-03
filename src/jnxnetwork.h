#ifndef __JNXNETWORK_H__
#define __JNXNETWORK_H__
/** @file jnxnetwork.h
 *  @brief The API for jnxlibc networking (includes TCP, UDP )
 *
 *  #include <jnxc_headers/jnxnetwork.h>
 */

/**
 * @brief Maximum buffer size allowed 
 */
#define MAXBUFFER 4028

#ifndef JNXNETWORK_RESPONSE_SUPRESS
#define DEFAULTRESPONSE "OK"
#endif
/*-----------------------------------------------------------------------------
 * Currently the library defaults to the use of IPv4, this can be overriden with USEIPV6 
 *-----------------------------------------------------------------------------*/
#ifndef JNXNETWORK_USEIPV6
#define ADDRESSFAMILY AF_INET
#else
#define ADDRESSFAMILY AF_INET6
#endif
/**
 * @brief This is a callback for sending a message to retrieve the response
 *
 */
typedef void (*jnx_network_send_message_callback)(char* msg);
/**
 *  @brief This is the callback for the received buffer from the network listener
 *  @warning User does not need to free client_ip_addr
 */
typedef void (*jnx_network_listener_callback)(char* msg,char*client_ip_addr);
/**
 *  @brief This is the callback for UDP datagram listening in the broadcast listener
 */
typedef void (*jnx_network_broadcast_callback)(char *msg);
 
/** @fn jnx_network_setup_listener(int port, void (*Callback)(char*))
 *  @brief Using port and a function pointer callback it will create a listener
 *  @param port selet target listener port
 *  @param Callback this is a function pointer to your jnx_network_listener_callback
 *  @return status code 0 success
 */
int jnx_network_setup_listener(int port, void (*Callback)(char*,char*));

/** @fn jnx_network_cancel_listener(void)
 * @brief Cancels the last set listener
 */
void jnx_network_cancel_listener(void);

/** @fn jnx_network_send_message(char* host, int port, char* msg)
 * @brief This sends a message via selected port to host
 *
 * @param host the ip address of the host you wish to connect to
 * @param port port number to connect to
 * @param msg message to be sent
 * @param callback_function gives a function pointer to a handler for any response text
 * @return returns 0 on success
 */
int jnx_network_send_message(char* host, int port, char* msg, void (*callback_function)(char*));

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
