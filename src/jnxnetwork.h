#ifndef __JNXNETWORK_H__
#define __JNXNETWORK_H__
/*-----------------------------------------------------------------------------
 *  This is the max char buffer size by default, also linked to read buffer size
 *-----------------------------------------------------------------------------*/
#define MAXBUFFER 4028
/*-----------------------------------------------------------------------------
 *  This is the function pointer to a callback function for received comms
 *-----------------------------------------------------------------------------*/
typedef void (*jnx_network_listener_callback)(char* msg);
typedef void (*jnx_network_broadcast_callback)(char *msg);
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_setup_listener
 *  Description:  Using port and a function pointer callback it will create a listener
 * =====================================================================================
 */
int jnx_network_setup_listener(int port, void (*Callback)(char*));

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_cancel_listener
 *  Description:  Cancels the listener
 * =====================================================================================
 */
void jnx_network_cancel_listener(void);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_send_message
 *  Description:  given host port and message, will send via tcp to target
 * =====================================================================================
 */
int jnx_network_send_message(char* host, int port, char* msg);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_local_ip
 *  Description:  Gets ip for the targetted interface
 * =====================================================================================
 */

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_network_send_broadcast
 *  Description:  Sends a udp datagram over a broadcast group
 * =====================================================================================
 */
void jnx_network_send_broadcast(int port,char *broadcastgroup,char *message);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_network_broadcast_listener
 *  Description:  Setups up a listener callback for a broadcast
 * =====================================================================================
 */
void jnx_network_broadcast_listener(int port,char *broadcastgroup, void(*jnx_network_broadcast_callback)(char*));
char* jnx_network_local_ip(char* interface);
#endif
