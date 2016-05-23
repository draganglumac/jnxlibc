/*
 * =====================================================================================
 *
 *       Filename:  jnxsocket.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/21/14 18:07:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnxsocket.h"
#include <corecrt_io.h>
#include<stdio.h>


jnx_socket *create_socket(jnx_unsigned_int type,\
    jnx_unsigned_int addrfamily, jnx_unsigned_int protocol) {

  JNXCHECK(addrfamily);
  JNXCHECK(type);
  JNXCHECK(addrfamily);
  struct WSAData wsaData;
  struct addrinfo hints;

  int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (res != 0) {
	  printf("WSAStartup failed with error: %d\n", res);
	  return 0;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  jnx_int32 sock = socket(addrfamily, type, protocol);
  JNXCHECK(sock);
  jnx_socket *s = malloc(sizeof(jnx_socket));
  s->isclosed = 0;
  s->isconnected = 0;
  s->addrfamily = addrfamily;
  s->socket = sock;
  s->stype = type;
  s->ipaddress = NULL;
  return s;
  /*
  jnx_int32 sock = socket(addrfamily,type,protocol);
  JNXCHECK(sock);
  jnx_int32 optval = 1;
  JNXCHECK(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof optval) == 0);
  jnx_socket *s = malloc(sizeof(jnx_socket));
  s->isclosed = 0;
  s->isconnected = 0;
  s->addrfamily = addrfamily;
  s->socket = sock;
  s->stype = type;
  s->ipaddress = NULL;
  return s;*/
}
void jnx_socket_close(jnx_socket *s) {
  JNXCHECK(s);
  if(!s->isclosed) {
    if(s->socket != -1) {
		closesocket(s->socket);
		WSACleanup();
      s->isclosed = 1;
    }
  }
}
void jnx_socket_destroy(jnx_socket **s) {
  JNXCHECK(*s);
  jnx_socket_close(*s);
  free(*s);
  *s = NULL;
}

