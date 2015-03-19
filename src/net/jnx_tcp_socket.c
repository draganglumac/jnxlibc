/*
 * jnx_tcp_socket.c
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
#include "jnxsocket.h"
#include "jnx_tcp_socket.h"

jnx_socket *jnx_socket_tcp_create(jnx_unsigned_int addrfamily) {
  return create_socket(SOCK_STREAM,addrfamily);
}
jnx_tcp_listener* jnx_socket_tcp_listener_create(jnx_char *port,
    jnx_unsigned_int family, jnx_int max_connections) {
  JNXCHECK(max_connections <= 200);
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  jnx_int on = 1,s;
  jnx_tcp_listener *l = malloc(sizeof(jnx_tcp_listener));
  l->socket = jnx_socket_tcp_create(family);
  l->hint_exit = 0;
  memset (&hints, 0, sizeof (struct addrinfo));
  hints.ai_family = family;
  hints.ai_socktype = l->socket->stype;
  hints.ai_flags = AI_PASSIVE;
  s = getaddrinfo (NULL, port, &hints, &result);
  if (s != 0)
  {
    fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
    JNXFAIL("getaddrinfo failure");  
  }
  s = ioctl(l->socket->socket,FIONBIO,(char*)&on);
  for (rp = result; rp != NULL; rp = rp->ai_next)
  {
    JNXCHECK(s == 0);
    s = bind (l->socket->socket, rp->ai_addr, rp->ai_addrlen);
    if (s == 0)
    {
      break;
    }
    jnx_socket_tcp_listener_destroy(&l);
    JNXFAIL("bind failure");
  }
  JNXCHECK(l->socket->socket);
  s = listen(l->socket->socket,max_connections);
  JNXCHECK(s == 0);
  l->ufds[0].fd = l->socket->socket;
  l->ufds[0].events = POLLIN;
  l->poll_timeout = 500;
  l->nfds = 1;
  return l;
}
void jnx_socket_tcp_listener_destroy(jnx_tcp_listener **listener) {
  jnx_socket_destroy(&(*listener)->socket);
  free(*listener);
  *listener = NULL;
}
void jnx_socket_tcp_listener_tick(jnx_tcp_listener* listener,
    jnx_tcp_listener_callback callback, void *args) {
  jnx_int rv = poll(listener->ufds,listener->nfds,listener->poll_timeout); 
  if (rv == -1) {
    perror("poll"); // error occurred in poll()
  } else if (rv == 0) {
    /* Timeout reached */
  }
  jnx_int i, current_size = listener->nfds;
  jnx_int close_conn = 0, compress_array = 0;
  for(i=0;i<current_size;++i) {
    if(listener->ufds[i].revents == 0) {
      continue;
    }
    if(listener->ufds[i].revents != POLLIN) {
      JNXFAIL("There shouldn't be a descriptor in this state");
      exit(0);
    }
    if(listener->ufds[i].fd == listener->socket->socket) {
      int new_fd = -1;
      do{
        new_fd = accept(listener->socket->socket,NULL,NULL);
        if(new_fd < 0) {
          if(errno != EWOULDBLOCK) {
            JNXFAIL("Could not accept");
            exit(0);
          }
          break;
        }
        jnx_char *incoming_address = jnx_socket_tcp_resolve_ipaddress(
            new_fd);
        if(incoming_address){
          JNX_LOG(0,"Incoming connection from %s on fd %d",incoming_address);
          free(incoming_address);
        }
        listener->ufds[listener->nfds].fd = new_fd;
        listener->ufds[listener->nfds].events = POLLIN;
        listener->nfds++;
      }while(new_fd != -1);
    }else {
      jnx_int rc = 0;
      JNX_LOG(0,"Descriptor is readable %d",listener->ufds[i].fd);
      close_conn = 0;
      compress_array = 0;
      char buffer[MAXBUFFER];
      bzero(buffer,MAXBUFFER);
      rc = recv(listener->ufds[i].fd,buffer,sizeof(buffer),0);
      if(rc < 0) {
        if(errno != EWOULDBLOCK){
          perror(" recv() failed");
          close_conn = 1;   
        }
      }
      if(rc == 0) {
        JNX_LOG(0,"Connection closed on fd %d",listener->ufds[i].fd);
        close_conn = 1;
      }
      /* Send callback */
      if(rc > 0) {
        jnx_uint8 *outbuffer = malloc((rc + 1) * sizeof(jnx_uint8));
        memset(outbuffer,0,rc + 1);
        memcpy(outbuffer,buffer,rc);
        callback(outbuffer,rc,listener->ufds[i].fd,args);
        free(outbuffer);
      }
      if(close_conn) {
        close(listener->ufds[i].fd);
        listener->ufds[i].fd = -1;
        compress_array = 1;
      }
      if(compress_array) {
        compress_array = 0;
        jnx_int j;
        for(j=i;j<listener->nfds;++j) {
          if(j+1 < listener->nfds) {
          listener->ufds[j] = listener->ufds[j+1];
          } 
        }
        listener->nfds--;
      }
    }
  }
}
void jnx_socket_tcp_listener_auto_tick(jnx_tcp_listener *listener,jnx_tcp_listener_callback callback, void *args) {
  while(!listener->hint_exit){
    jnx_socket_tcp_listener_tick(listener,callback,
        args);  
  }
}
jnx_char *jnx_socket_tcp_resolve_ipaddress(jnx_int32 socket) {
  jnx_char ipstr[INET6_ADDRSTRLEN];
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
jnx_size jnx_socket_tcp_send(jnx_socket *s, jnx_char *host,\
  jnx_char* port, jnx_uint8 *msg, jnx_size msg_len) {
  JNXCHECK(s);
  JNXCHECK(host);
  JNXCHECK(port);
  JNXCHECK(msg);
  JNXCHECK(msg_len);
  JNXCHECK(s->isclosed == 0);
  JNXCHECK(s->stype == SOCK_STREAM);
  JNXCHECK(s->addrfamily == AF_INET || s->addrfamily == AF_INET6);
  struct addrinfo hints, *res;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;

  jnx_int32 rg = 0;

  if((rg = getaddrinfo(host,port,&hints,&res)) != 0) {
    JNX_LOG(DEFAULT_CONTEXT,"%s\n",gai_strerror(rg));
    return 0;
  }
  if(!s->isconnected) {
    if(connect(s->socket,res->ai_addr,res->ai_addrlen) != 0) {
      perror("connect:");
      freeaddrinfo(res);
      return 0;
    }
    s->isconnected = 1;
  }
  freeaddrinfo(res);
  jnx_size tbytes = 0;
  jnx_size rbytes = msg_len;

  while(tbytes < rbytes) {
    jnx_size n = write(s->socket,msg,rbytes);
    if(n == -1) {
      perror("send:");
      return 0;
    }
    tbytes +=n;
    rbytes = msg_len - tbytes;
  }
  return tbytes;
}
jnx_size jnx_socket_tcp_send_with_receipt(jnx_socket *s,\
  jnx_char *host, jnx_char* port, jnx_uint8 *msg,\
  jnx_size msg_len,jnx_uint8 **out_receipt) {
  JNXCHECK(s);
  JNXCHECK(host);
  JNXCHECK(port);
  JNXCHECK(msg);
  JNXCHECK(msg_len);
  JNXCHECK(s->isclosed == 0);
  JNXCHECK(s->stype == SOCK_STREAM);
  struct addrinfo hints, *res;
  jnx_uint8 buffer[MAXBUFFER];
  memset(&hints,0,sizeof(hints));
  hints.ai_family = s->addrfamily;
  hints.ai_socktype = s->stype;
  *out_receipt = NULL;
  jnx_int32 rg = 0;
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
  jnx_size tbytes = 0;
  jnx_size rbytes = msg_len;

  while(tbytes < rbytes) {
    jnx_size n = write(s->socket,msg,rbytes);
    if(n == -1) {
      perror("send:");
      return 0;
    }
    tbytes +=n;
    rbytes = msg_len - tbytes;
  }
  memset(buffer,0,MAXBUFFER);
  jnx_size bytes_read = read(s->socket,buffer,MAXBUFFER);
  jnx_uint8 *out = calloc(bytes_read + 1, sizeof(jnx_uint8));
  memset(out,0,bytes_read + 1);
  memcpy(out,buffer,bytes_read + 1);
  *out_receipt = out;
  return bytes_read;
}
