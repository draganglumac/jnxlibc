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
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnxsocket.h"
jnx_socket *create_socket(jnx_unsigned_int type,\
    jnx_unsigned_int addrfamily) {
  JNXCHECK(addrfamily);
  JNXCHECK(type);
  JNXCHECK(addrfamily);
  jnx_int32 sock = socket(addrfamily,type,0);
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
  return s;
}
void jnx_socket_close(jnx_socket *s) {
  JNXCHECK(s);
  if(!s->isclosed) {
    if(s->socket != -1) {
      close(s->socket);
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

