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
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "jnxunixsocket.h"
#include "jnxcheck.h"
#define MAXBUFFER 1024

jnx_unix_socket *create_unix_socket(ssize_t stype, char*socket_path) {
  JNXCHECK(stype);
  JNXCHECK(socket_path);
  jnx_unix_socket *jus= calloc(1, sizeof(jnx_unix_socket));
  jus->isclosed = 0;
  jus->islisten = 0;
  int32_t sock = socket(AF_UNIX, stype, 0);
  if (sock == -1) {
    return NULL;
  }
  jus->socket = sock;
  (jus->address).sun_family = AF_UNIX;
  strncpy((jus->address).sun_path, socket_path, sizeof((jus->address).sun_path) - 1);
  jus->stype = stype;
  return jus;
}
jnx_unix_socket *jnx_unix_stream_socket_create(char *socket_path) {
  JNXCHECK(socket_path);
  return create_unix_socket(SOCK_STREAM, socket_path);
}
jnx_unix_socket *jnx_unix_datagram_socket_create(char *socket_path) {
  JNXCHECK(socket_path);
  return create_unix_socket(SOCK_DGRAM, socket_path);
}
void jnx_unix_socket_close(jnx_unix_socket *s) {
  JNXCHECK(s);
  if (!(s->isclosed)) {
    close(s->socket);
    if (s->islisten) {
      remove((s->address).sun_path);
    }
    s->isclosed = 1;
  }
}
void jnx_unix_socket_destroy(jnx_unix_socket **s) {
  JNXCHECK(*s);
  jnx_unix_socket_close(*s);
  free(*s);
  *s = NULL;
}
ssize_t write_to_stream_socket(jnx_unix_socket *s, uint8_t*msg, ssize_t msg_len) {
  JNXCHECK(s);
  size_t tbytes = 0;
  size_t rbytes = msg_len;

  while(tbytes < rbytes) {
    size_t n = write(s->socket,msg,rbytes);
    if(n == -1) {
      perror("jnx unix stream socket send");
      return 0;
    }
    tbytes +=n;
    rbytes = msg_len - tbytes;
  }
  return tbytes;
}
ssize_t jnx_unix_stream_socket_send(jnx_unix_socket *s, uint8_t*msg, ssize_t msg_len) {
  JNXCHECK(s);
  if (connect(s->socket,(struct sockaddr *)&(s->address),sizeof(struct sockaddr_un)) == -1) {
    perror("jnx unix stream socket connect");
    return 0;
  }

  return write_to_stream_socket(s, msg, msg_len);
}
ssize_t jnx_unix_datagram_socket_send(jnx_unix_socket *s, uint8_t*msg, ssize_t msg_len) {
  JNXCHECK(s);
  size_t tbytes = 0;
  size_t rbytes = msg_len;

  while(tbytes < rbytes) {
    size_t n = sendto(s->socket,msg,msg_len,0,(struct sockaddr *)&(s->address),sizeof(struct sockaddr_un));
    if(n == -1) {
      perror("jnx unix datagram socket sendto");
      return 0;
    }
    tbytes +=n;
    rbytes = msg_len - tbytes;
  }
  return tbytes;
}
int32_t bind_stream_socket(jnx_unix_socket *s) {
  JNXCHECK(s);
  if (bind(s->socket, (struct sockaddr *)&(s->address), sizeof(struct sockaddr_un)) == -1) {
    perror("jnx unix stream socket bind");
    return -1;
  }
  s->islisten = 1;
  return 0;
}
int32_t listen_on_stream_socket(jnx_unix_socket *s, ssize_t max_connections) {
  JNXCHECK(s);
  if (listen(s->socket, max_connections) == -1) {
    perror("jnx unix stream socket listen");
    return -1;
  }
  return 0;
}
jnx_unix_socket *accept_stream_socket_connection(jnx_unix_socket *s) {
  JNXCHECK(s);
  int32_t remote_sock;
  jnx_unix_socket *rs = jnx_unix_stream_socket_create("");
  socklen_t addr_len;
  if ((remote_sock = accept(s->socket,(struct sockaddr *)&(s->address), &addr_len)) == -1) {
    perror("jnx unix stream socket accept");
    jnx_unix_socket_destroy(&rs);
    return NULL;
  }
  else {
    jnx_unix_socket *rs = jnx_unix_stream_socket_create("");
    rs->socket = remote_sock;
    return rs;
  }	
}
int32_t read_stream_socket(jnx_unix_socket *s, uint8_t**out, int32_t *len) {
  JNXCHECK(s);
  uint8_t buffer[MAXBUFFER];
  memset(buffer,0,MAXBUFFER);
  FILE *fp = tmpfile();
  ssize_t bytesread = 0;
  while ((bytesread = read(s->socket, buffer, MAXBUFFER)) > 0) {
    fwrite(buffer, sizeof(uint8_t), bytesread, fp);
    bzero(buffer, MAXBUFFER);
    if (bytesread < MAXBUFFER) {
      break;
    }
  }
  if (bytesread == -1) {
    perror("jnx unix stream socket read");
    return -1;
  }
  *len = ftell(fp);
  rewind(fp);
  *out = (uint8_t*)(calloc(*len + 1, sizeof(uint8_t)));
  fread(*out, sizeof(uint8_t), *len, fp);
  fclose(fp);
  return 0;
}
int32_t jnx_unix_stream_socket_listen(jnx_unix_socket *s, ssize_t max_connections, stream_socket_listener_callback c) {
  JNXCHECK(s);
  if (bind_stream_socket(s) == -1) {
    return -1;
  }
  if (listen_on_stream_socket(s, max_connections) == -1) {
    return -1;
  }

  while(1) {
    jnx_unix_socket *remote_sock = accept_stream_socket_connection(s);
    if (remote_sock == NULL) {
      return -1;
    }

    uint8_t*out;
    int32_t len;
    if (read_stream_socket(remote_sock, &out, &len) == -1) {
      return -1;
    }

    int32_t ret = 0;
    if ((ret = c(out, len, remote_sock)) != 0) {
      JNX_LOG(DEFAULT_CONTEXT,"Exiting unix stream socket listener with %d\n",ret);
      free(out);
      jnx_unix_socket_destroy(&remote_sock);
      return 0;
    }
    free(out);
    jnx_unix_socket_destroy(&remote_sock);
  }

  return 0;
}
int32_t bind_datagram_socket(jnx_unix_socket *s) {
  JNXCHECK(s);
  if (bind(s->socket, (struct sockaddr *)&(s->address), sizeof(struct sockaddr_un)) == -1) {
    perror("jnx unix datagram socket bind");
    return -1;
  }
  s->islisten = 1;
  return 0;
}
int32_t receive_from_datagram_socket(jnx_unix_socket *s, jnx_unix_socket **remote_socket, uint8_t**out, int32_t *len) {
  JNXCHECK(s);

  uint8_t buffer[MAXBUFFER];
  memset(buffer,0,MAXBUFFER);
  jnx_unix_socket *rs = jnx_unix_datagram_socket_create("");
  socklen_t addr_len;
  ssize_t bytesread = recvfrom(s->socket,buffer,MAXBUFFER,0,(struct sockaddr *)&(rs->address),&addr_len);

  if (bytesread == -1) {
    perror("jnx unix datagram socket recvfrom");
    jnx_unix_socket_destroy(&rs);
    *remote_socket = NULL;
    return -1;
  }

  uint8_t*copy = calloc(bytesread, sizeof(uint8_t));
  memcpy(copy, buffer, bytesread);
  *out = copy;
  *len = bytesread;
  *remote_socket = rs;
  return 0;
}
int32_t jnx_unix_datagram_socket_listen(jnx_unix_socket *s, datagram_socket_listener_callback c) {
  JNXCHECK(s);
  if (bind_datagram_socket(s) == -1) {
    return -1;
  }

  while(1) {
    jnx_unix_socket *remote = NULL;
    uint8_t*out = NULL;
    int32_t len = 0;

    if (receive_from_datagram_socket(s, &remote, &out, &len) == -1) {
      return -1;
    }

    int32_t ret = 0;
    if ((ret = c(out, len, remote)) != 0) {
      JNX_LOG(DEFAULT_CONTEXT,"Exiting unix datagram socket listener with %d\n",ret);
      free(out);
      jnx_unix_socket_destroy(&remote);
      return 0;
    }
    free(out);
    jnx_unix_socket_destroy(&remote);
  }
  return 0;
}
