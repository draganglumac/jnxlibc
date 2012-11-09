#ifndef __NETWORK_H__
#define __NETWORK_H__
//This is the maximum char buffer size, though is also linked to the read buffer size
#define MAXBUFFER 1024
/*
This is designed to be a highly simplified C network function set
Currently operates via char buffer transmission
*/

//function pointer callback
//usage Callback = &function;
// (*Callback)(receiver_buffer);
typedef void (*Callback)(char*msg);
int setup_listener(int port, void (*Callback)(char*) );

int send_message(char *host,int port,char *msg);

#endif
