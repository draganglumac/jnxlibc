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
jnx_size internal_jnx_socket_udp_enable_multicast_listen(jnx_socket *s,
    jnx_char *ip,
    jnx_char *group) {
  JNXCHECK(s);
  JNXCHECK(ip);
  JNXCHECK(group);
  JNXCHECK(s->stype == SOCK_DGRAM);
  struct ip_mreq bgroup;
  bgroup.imr_multiaddr.s_addr = inet_addr(group);
  bgroup.imr_interface.s_addr = inet_addr(ip);
  JNX_LOG(0,"internal_jnx_socket_udp_enable_multicast_listen");
  if(setsockopt(s->socket,IPPROTO_IP,IP_ADD_MEMBERSHIP,(jnx_char*)&bgroup,
        sizeof(bgroup)) < 0) {
    perror("setsockopt:");
    return -1;
  }
  return 0;
}
jnx_size internal_jnx_socket_udp_enable_broadcast_send_or_listen(jnx_socket *s) {
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
jnx_socket *jnx_socket_udp_create(jnx_unsigned_int addrfamily) {
  return create_socket(SOCK_DGRAM,addrfamily);
}
jnx_udp_listener* jnx_socket_udp_listener_setup(jnx_char *port,
    jnx_unsigned_int family,int broadcast,int multicast,jnx_char *ip, 
    jnx_char *bgroup) {
  jnx_udp_listener *l = malloc(sizeof(jnx_udp_listener));
  l->socket = jnx_socket_udp_create(family);
  l->hint_exit = 0;
  struct addrinfo hints, *res, *p;
  memset(&hints,0,sizeof(struct addrinfo));
  hints.ai_family = family;
  hints.ai_socktype = l->socket->stype;
  hints.ai_flags = AI_PASSIVE;
  JNXCHECK(getaddrinfo(NULL,port,&hints,&res) == 0);
  jnx_int optval =0;
  p = res;

  while(p != NULL) {
    if (setsockopt(l->socket->socket, SOL_SOCKET, SO_REUSEADDR,
          &optval,sizeof(jnx_int32)) == -1) {
      perror("setsockopt");
      exit(1);
    }
    if (bind(l->socket->socket, p->ai_addr, p->ai_addrlen) == -1) {
      perror("server: bind");
      JNXFAIL("bind failure");
    }
    break;
    p= p->ai_next;
  }
  if(broadcast) {
    JNX_LOG(NULL,"Creating broadcast listener");
    internal_jnx_socket_udp_enable_broadcast_send_or_listen(l->socket); 
  }
  if(multicast) {
    JNXCHECK(bgroup);
    JNXCHECK(ip);
    JNX_LOG(NULL,"Creating multicast listener %s %s",ip,bgroup);
    internal_jnx_socket_udp_enable_multicast_listen(l->socket,ip,bgroup);
  }
  freeaddrinfo(res);
  return l;
}
jnx_udp_listener* jnx_socket_udp_listener_create(jnx_char *port,
      jnx_unsigned_int family) {
  return jnx_socket_udp_listener_setup(port,family,0,0,NULL,NULL);
}
jnx_udp_listener* jnx_socket_udp_listener_broadcast_create(jnx_char *port,
      jnx_unsigned_int family) {
  return jnx_socket_udp_listener_setup(port,family,1,0,NULL,NULL);
}
  
jnx_udp_listener* jnx_socket_udp_listener_multicast_create(jnx_char *port,
      jnx_unsigned_int family,jnx_char *ip,jnx_char *bgroup) {
  return jnx_socket_udp_listener_setup(port,family,0,1,ip,bgroup);
}
void jnx_socket_udp_listener_destroy(jnx_udp_listener **listener) {
  jnx_socket_destroy(&(*listener)->socket);
  free(*listener);
  *listener = NULL;
}
void jnx_socket_udp_listener_tick(jnx_udp_listener* listener,
    jnx_udp_listener_callback callback, void *args) {
  jnx_char buffer[MAX_UDP_BUFFER];
  memset(buffer,0,MAX_UDP_BUFFER);
  struct sockaddr_storage their_addr;
  socklen_t their_len = sizeof(their_addr);
  jnx_size bytesread = recvfrom(listener->socket->socket,buffer,
      MAX_UDP_BUFFER,0,(struct sockaddr *)&their_addr,(socklen_t*)&their_len);

  if(bytesread == -1) {
    perror("recvcfrom:");
    JNXFAIL("Unable to read bytes from recfrom");
  }
  jnx_uint8 *outbuffer = malloc((bytesread + 1) * sizeof(jnx_uint8));
  memset(outbuffer,0,bytesread + 1);
  memcpy(outbuffer,buffer,bytesread);

  callback(outbuffer,bytesread,args);
  free(outbuffer);
}
void jnx_socket_udp_listener_auto_tick(jnx_udp_listener *listener, 
    jnx_udp_listener_callback callback, void *args) {
  while(!listener->hint_exit){
    jnx_socket_udp_listener_tick(listener,callback,
        args);
  }
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
 jnx_size jnx_socket_udp_broadcast_send(jnx_socket *s, jnx_char *host,\
jnx_char* port, jnx_uint8 *msg, jnx_size msg_len) {
  internal_jnx_socket_udp_enable_broadcast_send_or_listen(s);
  return jnx_socket_udp_send(s,host,port,msg,msg_len);
}
jnx_size jnx_socket_udp_multicast_send(jnx_socket *s, jnx_char *group,\
  jnx_char* port, jnx_uint8 *msg, jnx_size msg_len) {
  //internal_jnx_socket_udp_enable_multicast_send(s,group);
  return jnx_socket_udp_send(s,group,port,msg,msg_len);
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
