#ifndef __NETWORK_H__
#define __NETWORK_H__
//This is the maximum char buffer size, though is also linked to the read buffer size
#define MAXBUFFER 1024
/*
This is designed to be a highly simplified C network function set
Currently operates via char buffer transmission
*/
///function pointer callback
///usage Callback = &function;
///(*Callback)(receiver_buffer);
typedef void (*Callback)(char*msg);
///Setup listener will create a socket listener on the selected port
///Requires both (int) port and void(*Callback)(char*) functor as arguments
int setup_listener(int port, void (*Callback)(char*) );
///Cancels the socket and closes it
void cancel_listener(void);
///Send message uses a simple charbuffer over write socket to the desired host-port
///Requires (char*)host (int) port and (char*) msg
int send_message(char *host,int port,char *msg);
#endif
