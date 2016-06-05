/*
 * jnx_tcp_socket.c
 * Copyright (C) 2015 Dragan Glumac <dragan.glumac@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnx_ipc_socket.h"
#include <corecrt_io.h>

#define MAXBUFFER 1024

static void jnx_ipc_socket_close(jnx_ipc_socket *s) {
 
}
static jnx_int32 bind_ipc_socket(jnx_ipc_socket *s) {
 
  return 0;
}
static jnx_int32 listen_on_ipc_socket(jnx_ipc_socket *s, jnx_int max_connections) {
 
  return 0;
}

// -- Public API starts here --
jnx_ipc_socket *jnx_socket_ipc_create(jnx_char *socket_path) {
	return NULL;
}
void jnx_ipc_socket_destroy(jnx_ipc_socket **s) {
 
}
jnx_ipc_listener* jnx_socket_ipc_listener_create(jnx_ipc_socket *s,
                                                 jnx_int max_connections) {
	return NULL;
}
void jnx_socket_ipc_listener_destroy(jnx_ipc_listener **listener) {
  jnx_ipc_socket_destroy(&(*listener)->socket);
  free(*listener);
  *listener = NULL;
}
void jnx_socket_ipc_listener_tick(jnx_ipc_listener* listener,
                                   jnx_ipc_listener_callback callback,
                                   void *args) {
  //jnx_int rv = poll(listener->ufds,listener->nfds,listener->poll_timeout);
  //if (rv == -1) {
  //  perror("jnx IPC socket poll"); // error occurred in poll()
  //} else if (rv == 0) {
  //  /* Timeout reached */
  //}
  //jnx_int i, current_size = listener->nfds;
  //jnx_int close_conn = 0, compress_array = 0;
  //for(i=0;i<current_size;++i) {
  //  if(listener->ufds[i].revents == 0) {
  //    continue;
  //  }
  //  if(listener->ufds[i].fd == listener->socket->socket) {
  //    int new_fd = -1;
  //    do{
  //      new_fd = accept(listener->socket->socket,NULL,NULL);
  //      if(new_fd < 0) {
  //        if(errno != EWOULDBLOCK) {
  //          JNXFAIL("jnx IPC socket could not accept");
  //          exit(0);
  //        }
  //        break;
  //      }
  //      listener->ufds[listener->nfds].fd = new_fd;
  //      listener->ufds[listener->nfds].events = POLLIN;
  //      listener->nfds++;
  //    }while(new_fd != -1);
  //  }else {
  //    jnx_size rc = 0;
  //    // JNXLOG(LDEBUG,"Descriptor is readable %d",listener->ufds[i].fd);
  //    close_conn = 0;
  //    compress_array = 0;
  //    char buffer[MAXBUFFER];
  //    bzero(buffer,MAXBUFFER);
  //    rc = recv(listener->ufds[i].fd,buffer,MAXBUFFER,0);
  //    if(rc == -1) {
  //      if(errno != EWOULDBLOCK){
  //        perror("jnx IPC socket recv() failed");
  //        close_conn = 1;   
  //      }
  //    }
  //    if(rc == 0) {
  //      // JNXLOG(0,"Connection closed on fd %d",listener->ufds[i].fd);
  //      close_conn = 1;
  //    }
  //    /* Send callback */
  //    if(rc > 0) {
  //      jnx_uint8 *outbuffer = malloc((rc + 1) * sizeof(jnx_uint8));
  //      memset(outbuffer,0,rc + 1);
  //      memcpy(outbuffer,buffer,rc);
  //      callback(outbuffer,rc,listener->ufds[i].fd,args);
  //      free(outbuffer);
  //    }
  //    if(close_conn) {
  //      close(listener->ufds[i].fd);
  //      listener->ufds[i].fd = -1;
  //      compress_array = 1;
  //    }
  //    if(compress_array) {
  //      compress_array = 0;
  //      jnx_int j;
  //      for(j=i;j<listener->nfds;++j) {
  //        if(j+1 < listener->nfds) {
  //        listener->ufds[j] = listener->ufds[j+1];
  //        } 
  //      }
  //      listener->nfds--;
  //    }
  //  }
  //}
}
void jnx_socket_ipc_listener_auto_tick(jnx_ipc_listener *listener,
                                       jnx_ipc_listener_callback callback,
                                       void *args) {
  // ToDo Introduce delay in the args parameter
  while(!listener->hint_exit) {
    jnx_socket_ipc_listener_tick(listener,callback, args);
  }
}
jnx_size jnx_socket_ipc_send(jnx_ipc_socket *s,
                             jnx_uint8 *msg, jnx_size msg_len) {
 /* JNXCHECK(s);
  JNXCHECK(msg);
  JNXCHECK(msg_len);
  JNXCHECK(s->isclosed == 0);

  if(!s->isconnected) {
    if(connect(s->socket,
               (struct sockaddr *)&(s->address),
               sizeof(struct sockaddr_un)) == -1) {
      perror("jnx IPC socket connect");
      return 0;
    }
    s->isconnected = 1;
  }

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
  return tbytes;*/
}
jnx_size jnx_socket_ipc_send_with_receipt(jnx_ipc_socket *s,
                                          jnx_uint8 *msg, jnx_size msg_len,
                                          jnx_uint8 **out_receipt) {
  //JNXCHECK(s);
  //JNXCHECK(msg);
  //JNXCHECK(msg_len);
  //JNXCHECK(s->isclosed == 0);

  //if(!s->isconnected) {
  //  if(connect(s->socket,
  //             (struct sockaddr *)&(s->address),
  //             sizeof(struct sockaddr_un)) == -1) {
  //    perror("jnx IPC socket connect");
  //    return 0;
  //  }
  //  s->isconnected = 1;
  //}

  //jnx_size tbytes = 0;
  //jnx_size rbytes = msg_len;
  //// write
  //while(tbytes < rbytes) {
  //  jnx_size n = write(s->socket,msg,rbytes);
  //  if(n == -1) {
  //    perror("send:");
  //    return 0;
  //  }
  //  tbytes +=n;
  //  rbytes = msg_len - tbytes;
  //}
  //// receive
  //jnx_uint8 buffer[MAXBUFFER];
  //memset(buffer,0,MAXBUFFER);
  //jnx_size bytes_read = read(s->socket,buffer,MAXBUFFER);
  //jnx_uint8 *out = calloc(bytes_read + 1, sizeof(jnx_uint8));
  //memset(out,0,bytes_read + 1);
  //memcpy(out,buffer,bytes_read + 1);
  //*out_receipt = out;
  //return bytes_read;
}
