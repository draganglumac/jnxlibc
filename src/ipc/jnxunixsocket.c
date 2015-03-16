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
#include <sys/un.h>
#include <arpa/inet.h>
#include "jnxunixsocket.h"
#include "jnxcheck.h"
#define MAXBUFFER 1024

jnx_unix_socket *create_unix_socket(jnx_size stype, jnx_char*socket_path) {
  JNXCHECK(stype);
  JNXCHECK(socket_path);
  jnx_unix_socket *jus= calloc(1, sizeof(jnx_unix_socket));
  jus->isclosed = 0;
  jus->islisten = 0;
  jnx_int32 sock = socket(AF_UNIX, stype, 0);
  if (sock == -1) {
    return NULL;
  }
  jus->socket = sock;
  (jus->address).sun_family = AF_UNIX;
  strncpy((jus->address).sun_path, socket_path, strlen(socket_path));
  jus->stype = stype;
  return jus;
}
jnx_unix_socket *jnx_unix_stream_socket_create(jnx_char *socket_path) {
  JNXCHECK(socket_path);
  return create_unix_socket(SOCK_STREAM, socket_path);
}
jnx_unix_socket *jnx_unix_datagram_socket_create(jnx_char *socket_path) {
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
jnx_size write_to_stream_socket(jnx_unix_socket *s, jnx_uint8*msg, jnx_size msg_len) {
  JNXCHECK(s);
  jnx_size tbytes = 0;
  jnx_size rbytes = msg_len;

  while(tbytes < rbytes) {
    jnx_size n = write(s->socket,msg,rbytes);
    if(n == -1) {
      perror("jnx unix stream socket send");
      return 0;
    }
    tbytes +=n;
    rbytes = msg_len - tbytes;
  }
  return tbytes;
}
jnx_size jnx_unix_stream_socket_send(jnx_unix_socket *s, jnx_uint8*msg, jnx_size msg_len) {
  JNXCHECK(s);
  if (connect(s->socket,(struct sockaddr *)&(s->address),sizeof(struct sockaddr_un)) == -1) {
    perror("jnx unix stream socket connect");
    return 0;
  }

  return write_to_stream_socket(s, msg, msg_len);
}
jnx_size jnx_unix_datagram_socket_send(jnx_unix_socket *s, jnx_uint8*msg, jnx_size msg_len) {
  JNXCHECK(s);
  jnx_size tbytes = 0;
  jnx_size rbytes = msg_len;

  while(tbytes < rbytes) {
    jnx_size n = sendto(s->socket,msg,msg_len,0,(struct sockaddr *)&(s->address),sizeof(struct sockaddr_un));
    if(n == -1) {
      perror("jnx unix datagram socket sendto");
      return 0;
    }
    tbytes +=n;
    rbytes = msg_len - tbytes;
  }
  return tbytes;
}
jnx_int32 bind_stream_socket(jnx_unix_socket *s) {
  JNXCHECK(s);
  if (bind(s->socket, (struct sockaddr *)&(s->address), sizeof(struct sockaddr_un)) == -1) {
    perror("jnx unix stream socket bind");
    return -1;
  }
  s->islisten = 1;
  return 0;
}
jnx_int32 listen_on_stream_socket(jnx_unix_socket *s, jnx_size max_connections) {
  JNXCHECK(s);
  if (listen(s->socket, max_connections) == -1) {
    perror("jnx unix stream socket listen");
    return -1;
  }
  return 0;
}
jnx_unix_socket *accept_stream_socket_connection(jnx_unix_socket *s) {
  JNXCHECK(s);
  jnx_uint32 remote_sock;
  jnx_unix_socket *rs = jnx_unix_stream_socket_create("");
  socklen_t addr_len = sizeof(s->address);
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
jnx_int32 read_stream_socket(jnx_unix_socket *s, jnx_uint8**out, jnx_uint32 *len) {
  JNXCHECK(s);
  jnx_uint8 buffer[MAXBUFFER];
  memset(buffer,0,MAXBUFFER);
  FILE *fp = tmpfile();
  jnx_ssize bytesread = 0;
  while ((bytesread = read(s->socket, buffer, MAXBUFFER)) > 0) {
    fwrite(buffer, sizeof(jnx_uint8), bytesread, fp);
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
  *out = (jnx_uint8*)(calloc(*len + 1, sizeof(jnx_uint8)));
  fread(*out, sizeof(jnx_uint8), *len, fp);
  fclose(fp);
  return 0;
}

jnx_int32 jnx_unix_stream_socket_listen_with_context(jnx_unix_socket *s, jnx_size max_connections, stream_socket_listener_callback_with_context cc, void *context) {
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

    jnx_uint8*out;
    jnx_uint32 len;
    if (read_stream_socket(remote_sock, &out, &len) == -1) {
      return -1;
    }

    jnx_uint32 ret = 0;
    if (context == NULL) {
      stream_socket_listener_callback c = (stream_socket_listener_callback) cc;
      if ((ret = c(out, len, remote_sock)) != 0) {
        JNX_LOG(DEFAULT_CONTEXT,"Exiting unix stream socket listener with %d\n",ret);
        free(out);
        jnx_unix_socket_destroy(&remote_sock);
        return 0;
      }
    }
    else {
      if ((ret = cc(out, len, remote_sock, context)) != 0) {
        JNX_LOG(DEFAULT_CONTEXT,"Exiting unix stream socket listener with %d\n",ret);
        free(out);
        jnx_unix_socket_destroy(&remote_sock);
        return 0;
      }
    }
    free(out);
    jnx_unix_socket_destroy(&remote_sock);
  }

  return 0;
}
jnx_int32 jnx_unix_stream_socket_listen(jnx_unix_socket *s, jnx_size max_connections, stream_socket_listener_callback c) {
  return jnx_unix_stream_socket_listen_with_context(
      s, max_connections,
      (stream_socket_listener_callback_with_context) c, NULL);
}
jnx_int32 bind_datagram_socket(jnx_unix_socket *s) {
  JNXCHECK(s);
  if (bind(s->socket, (struct sockaddr *)&(s->address), sizeof(struct sockaddr_un)) == -1) {
    perror("jnx unix datagram socket bind");
    return -1;
  }
  s->islisten = 1;
  return 0;
}
jnx_int32 receive_from_datagram_socket(jnx_unix_socket *s, jnx_unix_socket **remote_socket, jnx_uint8**out, jnx_uint32 *len) {
  JNXCHECK(s);

  jnx_uint8 buffer[MAXBUFFER];
  memset(buffer,0,MAXBUFFER);
  jnx_unix_socket *rs = jnx_unix_datagram_socket_create("");
  socklen_t addr_len;
  jnx_size bytesread = recvfrom(s->socket,buffer,MAXBUFFER,0,(struct sockaddr *)&(rs->address),&addr_len);

  if (bytesread == -1) {
    perror("jnx unix datagram socket recvfrom");
    jnx_unix_socket_destroy(&rs);
    *remote_socket = NULL;
    return -1;
  }

  jnx_uint8*copy = calloc(bytesread, sizeof(jnx_uint8));
  memcpy(copy, buffer, bytesread);
  *out = copy;
  *len = bytesread;
  *remote_socket = rs;
  return 0;
}
jnx_int32 jnx_unix_datagram_socket_listen_with_context(jnx_unix_socket *s, datagram_socket_listener_callback_with_context cc, void *context) {
  JNXCHECK(s);
  if (bind_datagram_socket(s) == -1) {
    return -1;
  }

  while(1) {
    jnx_unix_socket *remote = NULL;
    jnx_uint8*out = NULL;
    jnx_uint32 len = 0;

    if (receive_from_datagram_socket(s, &remote, &out, &len) == -1) {
      return -1;
    }

    jnx_uint32 ret = 0;
    if (context == NULL) {
      datagram_socket_listener_callback c = (datagram_socket_listener_callback) cc;
      if ((ret = c(out, len, remote)) != 0) {
        JNX_LOG(DEFAULT_CONTEXT,"Exiting unix datagram socket listener with %d\n",ret);
        free(out);
        jnx_unix_socket_destroy(&remote);
        return 0;
      }
    }
    else {
      if ((ret = cc(out, len, remote, context)) != 0) {
        JNX_LOG(DEFAULT_CONTEXT,"Exiting unix datagram socket listener with %d\n",ret);
        free(out);
        jnx_unix_socket_destroy(&remote);
        return 0;
      }

    }
    free(out);
    jnx_unix_socket_destroy(&remote);
  }
  return 0;
}
jnx_int32 jnx_unix_datagram_socket_listen(jnx_unix_socket *s, datagram_socket_listener_callback c) {
  return jnx_unix_datagram_socket_listen_with_context(
      s, (datagram_socket_listener_callback_with_context) c, NULL);
}
