#ifndef __JNXNETWORK_H__
#define __JNXNETWORK_H__
/*-----------------------------------------------------------------------------
 *  This is the max char buffer size by default, also linked to read buffer size
 *-----------------------------------------------------------------------------*/
#define MAXBUFFER 4028
/*-----------------------------------------------------------------------------
 *  This is the function pointer to a callback function for received comms
 *-----------------------------------------------------------------------------*/
typedef void (*jnx_listener_callback)(char* msg);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_setup_listener
 *  Description:  Using port and a function pointer callback it will create a listener
 * =====================================================================================
 */
int jnx_setup_listener(int port, void (*Callback)(char*));

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_cancel_listener
 *  Description:  Cancels the listener
 * =====================================================================================
 */
void jnx_cancel_listener(void);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_send_message
 *  Description:  given host port and message, will send via tcp to target
 * =====================================================================================
 */
int jnx_send_message(char* host, int port, char* msg);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_local_ip
 *  Description:  Gets ip for the targetted interface
 * =====================================================================================
 */
char* jnx_local_ip(char* interface);
#endif
