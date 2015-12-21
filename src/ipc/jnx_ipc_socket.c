/*
 * jnx_tcp_socket.c
 * Copyright (C) 2015 Dragan Glumac <dragan.glumac@gmail.com>
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
#include "jnx_ipc_socket.h"

void jnx_ipc_socket_close(jnx_ipc_socket *s) {
  JNXCHECK(s);
  if (!(s->isclosed)) {
    close(s->socket);
    if (s->islisten) {
      remove((s->address).sun_path);
    }
    s->isclosed = 1;
  }
}
jnx_int32 bind_stream_socket(jnx_ipc_socket *s) {
  JNXCHECK(s);
  if (bind(s->socket, (struct sockaddr *)&(s->address), sizeof(struct sockaddr_un)) == -1) {
    perror("jnx IPC socket bind");
    return -1;
  }
  s->islisten = 1;
  return 0;
}
jnx_int32 listen_on_stream_socket(jnx_ipc_socket *s, jnx_int max_connections) {
  JNXCHECK(s);
  if (listen(s->socket, max_connections) == -1) {
    perror("jnx IPC socket listen");
    return -1;
  }
  return 0;
}
jnx_ipc_socket *accept_stream_socket_connection(jnx_ipc_socket *s) {
  JNXCHECK(s);
  jnx_int32 remote_sock;
  jnx_ipc_socket *rs = jnx_socket_ipc_create("");
  socklen_t addr_len = sizeof(s->address);
  if ((remote_sock = accept(s->socket,(struct sockaddr *)&(s->address), &addr_len)) == -1) {
    perror("jnx IPC socket accept");
    jnx_ipc_socket_destroy(&rs);
    return NULL;
  }
  else {
    rs->socket = remote_sock;
    return rs;
  }
}

// -- Public API starts here --
jnx_ipc_socket *jnx_socket_ipc_create(jnx_char *socket_path) {
  JNXCHECK(socket_path);
  jnx_ipc_socket *jus= calloc(1, sizeof(jnx_ipc_socket));
  jus->isclosed = 0;
  jus->islisten = 0;
  jnx_int32 sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock == -1) {
    return NULL;
  }
  jus->socket = sock;
  (jus->address).sun_family = AF_UNIX;
  strncpy((jus->address).sun_path, socket_path, strlen(socket_path));
  return jus;
}
void jnx_ipc_socket_destroy(jnx_ipc_socket **s) {
  JNXCHECK(*s);
  jnx_ipc_socket_close(*s);
  free(*s);
  *s = NULL;
}
jnx_ipc_listener* jnx_socket_ipc_listener_create(jnx_ipc_socket *s,
                                                 jnx_int max_connections) {
  JNXCHECK(s);
  JNXCHECK(max_connections <= 200);
  jnx_int on = 1;
  jnx_ipc_listener *l = malloc(sizeof(jnx_ipc_listener));
  l->socket = s;
  l->hint_exit = 0;
  l->poll_timeout = 500;
  l->nfds = 1;

  ioctl(l->socket->socket,FIONBIO,(char*)&on);
  // attempt bind
  if (bind_stream_socket(s) == -1) {
    jnx_socket_ipc_listener_destroy(&l);
    return NULL;
  }
  // attempt listen
  if (listen_on_stream_socket(s, max_connections) == -1) {
    jnx_socket_ipc_listener_destroy(&l);
    return NULL;
  }
  l->ufds[0].fd = l->socket->socket;
  l->ufds[0].events = POLLIN;

  return l;
}
void jnx_socket_ipc_listener_destroy(jnx_ipc_listener **listener) {
  jnx_socket_destroy(&(*listener)->socket);
  free(*listener);
  *listener = NULL;
}
void jnx_socket_ipc_listener_tick(jnx_ipc_listener* listener,
                                   jnx_ipc_listener_callback callback,
                                   void *args) {
  // ToDo - implement
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
          JNXLOG(0,"Incoming connection from %s on fd %d",incoming_address);
          free(incoming_address);
        }
        listener->ufds[listener->nfds].fd = new_fd;
        listener->ufds[listener->nfds].events = POLLIN;
        listener->nfds++;
      }while(new_fd != -1);
    }else {
      jnx_int rc = 0;
      JNXLOG(0,"Descriptor is readable %d",listener->ufds[i].fd);
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
        JNXLOG(0,"Connection closed on fd %d",listener->ufds[i].fd);
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
void jnx_socket_ipc_listener_auto_tick(jnx_ipc_listener *listener,
                                       jnx_ipc_listener_callback callback,
                                       void *args) {
  // ToDo - implement
  while(!listener->hint_exit) {
    jnx_socket_ipc_listener_tick(listener,callback, args);
  }
}
jnx_size jnx_socket_ipc_send(jnx_ipc_socket *s,
                             jnx_uint8 *msg, jnx_size msg_len) {
  // ToDo - implement
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
    JNXLOG(LDEBUG,"%s\n",gai_strerror(rg));
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
jnx_size jnx_socket_ipc_send_with_receipt(jnx_ipc_socket *s,
                                          jnx_uint8 *msg, jnx_size msg_len,
                                          jnx_uint8 **out_receipt) {
  // ToDo - implement
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
    JNXLOG(LDEBUG,"%s\n",gai_strerror(rg));
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
