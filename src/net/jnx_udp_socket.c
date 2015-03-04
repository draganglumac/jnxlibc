/*
 * jnx_udp_socket.c
 * Copyright (C) 2015 tibbar <tibbar@debian>
 *
 * Distributed under terms of the MIT license.
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
#include "jnx_udp_socket.h"
#define KILOBYTE 1024
#define MAXBUFFER KILOBYTE * 10 
#define MAX_UDP_BUFFER KILOBYTE * 65

jnx_socket *jnx_socket_udp_create(jnx_unsigned_int addrfamily) {
  return create_socket(SOCK_DGRAM,addrfamily);
}
jnx_size jnx_socket_udp_enable_multicast_send(jnx_socket *s,\
  jnx_char *interface, int ignore_local) {
  JNXCHECK(s);
  JNXCHECK(interface);
  JNXCHECK(s->stype == SOCK_DGRAM);
  jnx_int32 optval = 0;
  struct in_addr localinterface;
  if(ignore_local) {
    if(setsockopt(s->socket,IPPROTO_IP,IP_MULTICAST_LOOP,&optval, sizeof(optval)) != 0) {
      perror("setsockopt:");
      return -1;
    }
  }
  localinterface.s_addr = inet_addr(interface);
  if(setsockopt(s->socket,IPPROTO_IP,IP_MULTICAST_IF,(jnx_char*)&localinterface,
        sizeof(localinterface)) < 0) {
    perror("setsockopt:");
    return -1;
  }
  //Note: This is due to bug in getaddrinfo
  s->addrfamily = AF_UNSPEC;
  return 0;
}
jnx_size jnx_socket_udp_enable_multicast_listen(jnx_socket *s,\
  jnx_char *interface, jnx_char *group) {
  JNXCHECK(s);
  JNXCHECK(interface);
  JNXCHECK(group);
  JNXCHECK(s->stype == SOCK_DGRAM);
  struct ip_mreq bgroup;
  bgroup.imr_multiaddr.s_addr = inet_addr(group);
  bgroup.imr_interface.s_addr = inet_addr(interface);
  if(setsockopt(s->socket,IPPROTO_IP,IP_ADD_MEMBERSHIP,(jnx_char*)&bgroup,
        sizeof(bgroup)) < 0) {
    perror("setsockopt:");
    return -1;
  }
  //Note: This is due to bug in getaddrinfo
  s->addrfamily = AF_UNSPEC;
  return 0;
}
jnx_size jnx_socket_udp_send(jnx_socket *s,\
  jnx_char *host, jnx_char* port, jnx_uint8 *msg,\
  jnx_size msg_len) {
  JNXCHECK(s);
  JNXCHECK(host);
  JNXCHECK(port);
  JNXCHECK(msg);
  JNXCHECK(msg_len);
  JNXCHECK(s->isclosed == 0);
  JNXCHECK(s->stype == SOCK_DGRAM);
  struct addrinfo hints, *res;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;
  hints.ai_flags =0;
  hints.ai_protocol = IPPROTO_UDP;
  hints.ai_next = NULL;

  jnx_int32 rg = 0;
  if((rg = getaddrinfo(host,port,&hints,&res)) != 0) {
    JNX_LOG(DEFAULT_CONTEXT,"%s\n",gai_strerror(rg));
    return 0;
  }
  jnx_size tbytes = 0;
  jnx_size rbytes = msg_len;
  if(rbytes > MAX_UDP_BUFFER) {
    JNX_LOG(DEFAULT_CONTEXT,"Message exceeds max UDP packet size\n");
    freeaddrinfo(res);
    return 0;
  }
  while(tbytes < rbytes) {

    jnx_size n = sendto(s->socket,msg,msg_len,0,res->ai_addr,res->ai_addrlen);
    if(n == -1) {
      freeaddrinfo(res);
      perror("send:");
      return 0;
    }
    tbytes +=n;
    rbytes = msg_len - tbytes;
  }
  freeaddrinfo(res);
  return tbytes;
}


jnx_int32 jnx_socket_udp_listen(jnx_socket *s, jnx_char* port,\
  jnx_size max_connections, udp_socket_listener_callback c) {
  return jnx_socket_udp_listen_with_context(s, port, max_connections,\
    (udp_socket_listener_callback_with_context) c, NULL);
}
jnx_int32 jnx_socket_udp_listen_with_context(jnx_socket *s, jnx_char* port,\
  jnx_size max_connections,\
  udp_socket_listener_callback_with_context c, void *context) {
  JNXCHECK(s);
  JNXCHECK(port);
  JNXCHECK(s->isclosed == 0);
  JNXCHECK(s->stype == SOCK_DGRAM);
  jnx_int32 optval = 1;
  struct addrinfo hints, *res, *p;
  struct sockaddr_storage their_addr;
  socklen_t their_len = sizeof(their_addr);
  jnx_uint8 buffer[MAX_UDP_BUFFER];
  memset(&hints,0,sizeof(struct addrinfo));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;
  hints.ai_flags = AI_PASSIVE;
  JNXCHECK(getaddrinfo(NULL,port,&hints,&res) == 0);
  p = res;
  while(p != NULL) {
    if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR,
      &optval,sizeof(jnx_int32)) == -1) {
      perror("setsockopt");
      exit(1);
    }
    if (bind(s->socket, p->ai_addr, p->ai_addrlen) == -1) {
      perror("server: bind");
      return -1;
    }
    break;
    p= p->ai_next;
  }
  freeaddrinfo(res);
  while(1) {
    memset(buffer,0,MAX_UDP_BUFFER);
    jnx_size bytesread = recvfrom(s->socket,buffer,
      MAX_UDP_BUFFER,0,(struct sockaddr *)&their_addr,(socklen_t*)&their_len);

    if(bytesread == -1) {
      perror("recvcfrom:");
      return -1;
    }
    jnx_int32 ret = 0;
    jnx_uint8 *outbuffer = malloc((bytesread + 1) * sizeof(jnx_uint8));
    memset(outbuffer,0,bytesread + 1);
    memcpy(outbuffer,buffer,bytesread);
    if(context != NULL) {
      if((ret = c(outbuffer,bytesread,s,context)) != 0) {
        return 0;
      }
    }
    else {
      udp_socket_listener_callback cb = (udp_socket_listener_callback) c;
      if((ret = cb(outbuffer,bytesread,s)) != 0) {
        return 0;
      }
    }
  }
  return -1;
}
jnx_size jnx_socket_udp_enable_broadcast_send_or_listen(jnx_socket *s) {
  JNXCHECK(s);
  JNXCHECK(s->stype == SOCK_DGRAM);
  JNXCHECK(s->addrfamily == AF_INET);
  jnx_int32 optval = 1;
  if(setsockopt(s->socket,SOL_SOCKET,SO_BROADCAST,&optval, sizeof(optval)) != 0) {
    perror("setsockopt:");
    return -1;
  }
  return 0;
}
jnx_char *jnx_socket_udp_resolve_ipaddress(struct sockaddr_storage sa) {
  jnx_char str[INET6_ADDRSTRLEN];
  memset(str,0,INET6_ADDRSTRLEN);
  switch (((struct sockaddr*)&sa)->sa_family) {
    case AF_INET:
      inet_ntop(AF_INET, &(((struct sockaddr_in*)&sa)->sin_addr),str,INET_ADDRSTRLEN);
      return strndup(str,strlen(str));
      break;
    case AF_INET6:
      inet_ntop(AF_INET6, &(((struct sockaddr_in6*)&sa)->sin6_addr),str,INET6_ADDRSTRLEN);
      return strndup(str,strlen(str));
      break;
  }
  return NULL;
}
