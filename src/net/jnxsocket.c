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
#define KILOBYTE 1024
#define MAXBUFFER KILOBYTE * 10 
#define MAX_UDP_BUFFER KILOBYTE * 65
jnx_socket *create_socket(unsigned int type,unsigned int addrfamily) {
  JNXCHECK(addrfamily);
  JNXCHECK(type);
  JNXCHECK(type);
  JNXCHECK(addrfamily);
  int32_t sock = socket(addrfamily,type,0);
  JNXCHECK(sock);
  int32_t optval = 1;
  setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof optval);
  jnx_socket *s = malloc(sizeof(jnx_socket));
  s->isclosed = 0;
  s->addrfamily = addrfamily;
  s->socket = sock;
  s->stype = type;
  s->ipaddress = NULL;
  return s;
}
jnx_socket *jnx_socket_tcp_create(unsigned int addrfamily) {
  return create_socket(SOCK_STREAM,addrfamily);
}
jnx_socket *jnx_socket_udp_create(unsigned int addrfamily) {
  return create_socket(SOCK_DGRAM,addrfamily);
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
size_t jnx_socket_udp_enable_broadcast_send_or_listen(jnx_socket *s) {
  JNXCHECK(s);
  JNXCHECK(s->stype == SOCK_DGRAM);
  JNXCHECK(s->addrfamily == AF_INET);
  int32_t optval = 1;
  if(setsockopt(s->socket,SOL_SOCKET,SO_BROADCAST,&optval, sizeof(optval)) != 0) {
    perror("setsockopt:");
    return -1;
  }
  return 0;
}
size_t jnx_socket_udp_enable_multicast_send(jnx_socket *s, char *interface, int ignore_local) {
  JNXCHECK(s);
  JNXCHECK(interface);
  JNXCHECK(s->stype == SOCK_DGRAM);
  int32_t optval = 0;
  struct in_addr localinterface;
  if(ignore_local) {
    if(setsockopt(s->socket,IPPROTO_IP,IP_MULTICAST_LOOP,&optval, sizeof(optval)) != 0) {
      perror("setsockopt:");
      return -1;
    }
  }
  localinterface.s_addr = inet_addr(interface);
  if(setsockopt(s->socket,IPPROTO_IP,IP_MULTICAST_IF,(char*)&localinterface,
        sizeof(localinterface)) < 0) {
    perror("setsockopt:");
    return -1;
  }
  //Note: This is due to bug in getaddrinfo
  s->addrfamily = AF_UNSPEC;
  return 0;
}
size_t jnx_socket_udp_enable_multicast_listen(jnx_socket *s, char *interface, char *group) {
  JNXCHECK(s);
  JNXCHECK(interface);
  JNXCHECK(group);
  JNXCHECK(s->stype == SOCK_DGRAM);
  struct ip_mreq bgroup;
  bgroup.imr_multiaddr.s_addr = inet_addr(group);
  bgroup.imr_interface.s_addr = inet_addr(interface);
  if(setsockopt(s->socket,IPPROTO_IP,IP_ADD_MEMBERSHIP,(char*)&bgroup,
        sizeof(bgroup)) < 0) {
    perror("setsockopt:");
    return -1;
  }
  //Note: This is due to bug in getaddrinfo
  s->addrfamily = AF_UNSPEC;
  return 0;
}
char *jnx_socket_tcp_resolve_ipaddress(int32_t socket) {
  char ipstr[INET6_ADDRSTRLEN];
  socklen_t len;
  struct sockaddr_storage addr;
  len = sizeof(addr);

  if(getpeername(socket,(struct sockaddr*)&addr,&len) == -1) {
    perror("getpeername:");
    return NULL;
  }
  if(addr.ss_family != AF_INET && addr.ss_family != AF_INET6) {
    return NULL;
  }
  if(addr.ss_family == AF_INET) {
    struct sockaddr_in *s = (struct sockaddr_in*)&addr;
    inet_ntop(AF_INET,&s->sin_addr,ipstr,sizeof(ipstr));
  } else {
    struct sockaddr_in6 *s = (struct sockaddr_in6*)&addr;
    inet_ntop(AF_INET6,&s->sin6_addr,ipstr,sizeof(ipstr));
  }
  return strdup(ipstr);
}
char *jnx_socket_udp_resolve_ipaddress(struct sockaddr_storage sa) {
  char str[INET6_ADDRSTRLEN];
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
ssize_t jnx_socket_tcp_send(jnx_socket *s, char *host, char* port, uint8_t *msg, ssize_t msg_len) {
  JNXCHECK(s);
  JNXCHECK(host);
  JNXCHECK(port);
  JNXCHECK(msg);
  JNXCHECK(msg_len);
  JNXCHECK(s->isclosed == 0);
  JNXCHECK(s->stype == SOCK_STREAM);
  struct addrinfo hints, *res;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;

  int32_t rg = 0;
  if((rg = getaddrinfo(host,port,&hints,&res)) != 0) {
    JNX_LOG(DEFAULT_CONTEXT,"%s\n",gai_strerror(rg));
    return 0;
  }
  if(connect(s->socket,res->ai_addr,res->ai_addrlen) != 0) {
    perror("connect:");
    freeaddrinfo(res);
    return 0;
  }
  freeaddrinfo(res);
  size_t tbytes = 0;
  size_t rbytes = msg_len;

  while(tbytes < rbytes) {
    size_t n = write(s->socket,msg,rbytes);
    if(n == -1) {
      perror("send:");
      return 0;
    }
    tbytes +=n;
    rbytes = msg_len - tbytes;
  }
  return tbytes;
}
ssize_t jnx_socket_tcp_send_with_receipt(jnx_socket *s, char *host, char* port, uint8_t *msg, ssize_t msg_len,uint8_t **out_receipt) {
  JNXCHECK(s);
  JNXCHECK(host);
  JNXCHECK(port);
  JNXCHECK(msg);
  JNXCHECK(msg_len);
  JNXCHECK(s->isclosed == 0);
  JNXCHECK(s->stype == SOCK_STREAM);
  struct addrinfo hints, *res;
  uint8_t buffer[MAXBUFFER];
  memset(&hints,0,sizeof(hints));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;
  *out_receipt = NULL;
  int32_t rg = 0;
  if((rg = getaddrinfo(host,port,&hints,&res)) != 0) {
    JNX_LOG(DEFAULT_CONTEXT,"%s\n",gai_strerror(rg));
    return 0;
  }
  if(connect(s->socket,res->ai_addr,res->ai_addrlen) != 0) {
    perror("connect:");
    freeaddrinfo(res);
    return 0;
  }
  freeaddrinfo(res);
  size_t tbytes = 0;
  size_t rbytes = msg_len;

  while(tbytes < rbytes) {
    size_t n = write(s->socket,msg,rbytes);
    if(n == -1) {
      perror("send:");
      return 0;
    }
    tbytes +=n;
    rbytes = msg_len - tbytes;
  }
  memset(buffer,0,MAXBUFFER);
  FILE *fp = tmpfile();
  if(fp == NULL) {
    perror("tmpfile:");
    return -1;
  }
  size_t bytesread = read(s->socket,buffer,MAXBUFFER);
  fwrite(buffer,sizeof(uint8_t),bytesread,fp);

  while(bytesread > 0) {
    memset(buffer,0,MAXBUFFER);
    bytesread = read(s->socket,buffer,MAXBUFFER);
    if(bytesread == -1) {
      perror("read:");
      fclose(fp);
      return -1;
    }
    if(bytesread > 0) {
      fwrite(buffer,sizeof(uint8_t),bytesread,fp);
    }
  }
  int32_t len = ftell(fp);
  rewind(fp);
  uint8_t *out = calloc(len + 1, sizeof(uint8_t));
  fread(out,sizeof(uint8_t),len,fp);
  fclose(fp);
  *out_receipt = out;
  return len;
}
ssize_t jnx_socket_udp_send(jnx_socket *s, char *host, char* port, uint8_t *msg, ssize_t msg_len) {
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

  int32_t rg = 0;
  if((rg = getaddrinfo(host,port,&hints,&res)) != 0) {
    JNX_LOG(DEFAULT_CONTEXT,"%s\n",gai_strerror(rg));
    return 0;
  }
  size_t tbytes = 0;
  size_t rbytes = msg_len;
  if(rbytes > MAX_UDP_BUFFER) {
    JNX_LOG(DEFAULT_CONTEXT,"Message exceeds max UDP packet size\n");
    freeaddrinfo(res);
    return 0;
  }
  while(tbytes < rbytes) {
    size_t n = sendto(s->socket,msg,msg_len,0,res->ai_addr,res->ai_addrlen);
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
int32_t jnx_socket_tcp_listen(jnx_socket *s, char* port, ssize_t max_connections, tcp_socket_listener_callback c) {
  JNXCHECK(s);
  JNXCHECK(port);
  JNXCHECK(s->isclosed == 0);
  JNXCHECK(s->stype == SOCK_STREAM);
  int32_t optval = 1;
  struct addrinfo hints, *res, *p;
  struct sockaddr_storage their_addr;
  uint8_t buffer[MAXBUFFER];
  memset(&hints,0,sizeof(hints));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;
  getaddrinfo(NULL,port,&hints,&res);
  p = res;
  while(p != NULL) {
    if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &optval,sizeof(int32_t)) == -1) {
      perror("setsockopt");
      exit(1);
    }
    if (bind(s->socket, p->ai_addr, p->ai_addrlen) == -1) {
      perror("server: bind");
      return -1;
    }
    break;

    p = p->ai_next;
  }
  freeaddrinfo(res);
  listen(s->socket,max_connections);
  while(1) {
    socklen_t addr_size = sizeof(their_addr);
    int32_t recfd = accept(s->socket,(struct sockaddr*)&their_addr,&addr_size);
    if(recfd < 0) {
      JNX_LOG(DEFAULT_CONTEXT,"accept: %s",strerror(errno));
      perror("accept:");
      return -1;
    }
    memset(buffer,0,MAXBUFFER);
    FILE *fp = tmpfile();
    if(fp == NULL) {
      perror("tmpfile:");
      return -1;
    }
    size_t bytesread = read(recfd,buffer,MAXBUFFER);
    fwrite(buffer,sizeof(uint8_t),bytesread,fp);

    while(bytesread > 0) {
      memset(buffer,0,MAXBUFFER);
      bytesread = read(recfd,buffer,MAXBUFFER);
      if(bytesread == -1) {
        perror("read:");
        fclose(fp);
        return -1;
      }
      if(bytesread > 0) {
        fwrite(buffer,sizeof(uint8_t),bytesread,fp);
      }
    }
    int32_t len = ftell(fp);
    rewind(fp);
    uint8_t *out = calloc(len + 1, sizeof(uint8_t));
    fread(out,sizeof(uint8_t),len,fp);
    fclose(fp);

    int32_t ret = 0;
    if((ret = c(out,len,s)) != 0) {
      return 0;
    }
  }
  return 0;
}
int32_t jnx_socket_udp_listen(jnx_socket *s, char* port, ssize_t max_connections, udp_socket_listener_callback c) {
  JNXCHECK(s);
  JNXCHECK(port);
  JNXCHECK(s->isclosed == 0);
  JNXCHECK(s->stype == SOCK_DGRAM);
  int32_t optval = 1;
  struct addrinfo hints, *res, *p;
  struct sockaddr_storage their_addr;
  socklen_t their_len = sizeof(their_addr);
  uint8_t buffer[MAX_UDP_BUFFER];
  memset(&hints,0,sizeof(struct addrinfo));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;
  hints.ai_flags = AI_PASSIVE;
  getaddrinfo(NULL,port,&hints,&res);
  p = res;
  while(p != NULL) {
    if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &optval,sizeof(int32_t)) == -1) {
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
    size_t bytesread = recvfrom(s->socket,buffer,MAX_UDP_BUFFER,0,(struct sockaddr *)&their_addr,(socklen_t*)&their_len);

    if(bytesread == -1) {
      perror("recvcfrom:");
      return -1;
    }
    int32_t ret = 0;
    uint8_t *outbuffer = malloc(bytesread * sizeof(uint8_t));
    memcpy(outbuffer,buffer,bytesread);
    if((ret = c(outbuffer,bytesread,s)) != 0) {
      return 0;
    }
  }
  return -1;
}
