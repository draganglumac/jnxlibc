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
 *         Author:  jonesax (),
 *   Organization:
 *
 * =====================================================================================
 */
#if defined(WIN32)

#else
#include <arpa/inet.h>
#endif
#include <assert.h>
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
#include "jnxsocket.h"
#define MAXBUFFER 1024
jnx_socket *create_socket(unsigned int type,unsigned int addrfamily) {
    assert(type);
    assert(addrfamily);
    int sock = socket(addrfamily,type,0);
    if(sock < 0) {
        return NULL;
    }
    int optval = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof optval);
    jnx_socket *s = malloc(sizeof(jnx_socket));
    s->isclosed = 0;
    s->addrfamily = addrfamily;
    s->socket = sock;
    s->stype = type;
    return s;
}
jnx_socket *jnx_socket_tcp_create(unsigned int addrfamily) {
    return create_socket(SOCK_STREAM,addrfamily);
}
jnx_socket *jnx_socket_udp_create(unsigned int addrfamily) {
    return create_socket(SOCK_DGRAM,addrfamily);
}
void jnx_socket_close(jnx_socket *s) {
    if(!s->isclosed) {
        if(s->socket != -1) {
            close(s->socket);
            s->isclosed = 1;
        }
    }
}
void jnx_socket_destroy(jnx_socket **s) {
    jnx_socket_close(*s);
    free(*s);
    *s = NULL;
}
size_t jnx_socket_udp_enable_broadcast_send_or_listen(jnx_socket *s) {
    assert(s->stype == SOCK_DGRAM);
    assert(s->addrfamily == AF_INET);
    int optval = 1;
    if(setsockopt(s->socket,SOL_SOCKET,SO_BROADCAST,&optval, sizeof(optval)) != 0) {
        perror("setsockopt:");
        return -1;
    }
    return 0;
}
size_t jnx_socket_udp_enable_multicast_send(jnx_socket *s, char *interface, int ignore_local) {
    assert(interface);
    assert(s->stype == SOCK_DGRAM);
    int optval = 0;
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
    assert(interface);
    assert(group);
    assert(s->stype == SOCK_DGRAM);
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
char *jnx_socket_tcp_resolve_ipaddress(int socket) {
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
    bzero(str,INET6_ADDRSTRLEN);
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
size_t jnx_socket_tcp_send(jnx_socket *s, char *host, char* port, char *msg, ssize_t msg_len) {
    assert(s);
    assert(host);
    assert(port);
    assert(msg);
    assert(msg_len);
    assert(s->isclosed == 0);
    assert(s->stype == SOCK_STREAM);
    struct addrinfo hints, *res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = s->addrfamily;
    hints.ai_socktype = s->stype;

    getaddrinfo(host,port,&hints,&res);

    if(connect(s->socket,res->ai_addr,res->ai_addrlen) != 0) {
        perror("connect:");
        freeaddrinfo(res);
        return -1;
    }
    freeaddrinfo(res);

    size_t tbytes = 0;
    size_t rbytes = msg_len;

    while(tbytes < rbytes) {
        size_t n = write(s->socket,msg,rbytes);
        if(n == -1) {
            perror("send:");
            return -1;
        }
        tbytes +=n;
        rbytes = msg_len - tbytes;
    }
    return tbytes;
}
size_t jnx_socket_udp_send(jnx_socket *s, char *host, char* port, char *msg, ssize_t msg_len) {
    assert(s);
    assert(host);
    assert(port);
    assert(msg);
    assert(msg_len);
    assert(s->isclosed == 0);
    assert(s->stype == SOCK_DGRAM);
    struct addrinfo hints, *res;
    memset(&hints,0,sizeof(hints));
    hints.ai_family = s->addrfamily;
    hints.ai_socktype = s->stype;
    hints.ai_flags =0;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_next = NULL;

    int rg = 0;
    if((rg = getaddrinfo(host,port,&hints,&res)) != 0) {
        printf("%s\n",gai_strerror(rg));
        return -1;
    }
    size_t tbytes = 0;
    size_t rbytes = msg_len;

    while(tbytes < rbytes) {
        size_t n = sendto(s->socket,msg,msg_len,0,res->ai_addr,res->ai_addrlen);
        if(n == -1) {
            freeaddrinfo(res);
            perror("send:");
            return -1;
        }
        tbytes +=n;
        rbytes = msg_len - tbytes;
    }
    freeaddrinfo(res);
    return tbytes;
}
size_t jnx_socket_tcp_listen(jnx_socket *s, char* port, ssize_t max_connections, tcp_socket_listener_callback c) {
    assert(s);
    assert(port);
    assert(s->isclosed == 0);
    assert(s->stype == SOCK_STREAM);
    int optval = 1;
    struct addrinfo hints, *res, *p;
    struct sockaddr_storage their_addr;
    char buffer[MAXBUFFER];
    memset(&hints,0,sizeof(hints));
    hints.ai_family = s->addrfamily;
    hints.ai_socktype = s->stype;

    getaddrinfo(NULL,port,&hints,&res);

    p = res;
    while(p != NULL) {
        if((s->socket = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &optval,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        if (bind(s->socket, p->ai_addr, p->ai_addrlen) == -1) {
            perror("server: bind");
            continue;
        }
        break;

        p = p->ai_next;
    }
    freeaddrinfo(res);
    listen(s->socket,max_connections);
    while(1) {
        socklen_t addr_size = sizeof(their_addr);
        int recfd = accept(s->socket,(struct sockaddr*)&their_addr,&addr_size);
        if(recfd < 0) {
            JNX_LOGC(JLOG_ALERT,"accept: %s",strerror(errno));
            return -1;
        }
        bzero(buffer,MAXBUFFER);
        FILE *fp = tmpfile();
        size_t bytesread = read(recfd,buffer,MAXBUFFER);
        fwrite(buffer,sizeof(char),bytesread,fp);

        while(bytesread > 0) {
            bzero(buffer,MAXBUFFER);
            bytesread = read(recfd,buffer,MAXBUFFER);
            if(bytesread == -1) {
                perror("read:");
                fclose(fp);
                return -1;
            }
            if(bytesread > 0) {
                fwrite(buffer,sizeof(char),bytesread,fp);
            }
        }
        int len = ftell(fp);
        rewind(fp);
        char *out = calloc(len + 1, sizeof(char));
        fread(out,1,len,fp);
        fclose(fp);

        int ret = 0;
        if((ret = c(out,len,jnx_socket_tcp_resolve_ipaddress(recfd))) != 0) {
            printf("Exiting tcp listener with %d\n",ret);
            return 0;
        }
    }
    return 0;
}
size_t jnx_socket_udp_listen(jnx_socket *s, char* port, ssize_t max_connections, udp_socket_listener_callback c) {
    assert(s);
    assert(port);
    assert(s->isclosed == 0);
    assert(s->stype == SOCK_DGRAM);
    int optval = 1;
    struct addrinfo hints, *res, *p;
    struct sockaddr_storage their_addr;
    socklen_t their_len = sizeof(their_addr);
    char buffer[MAXBUFFER];
    memset(&hints,0,sizeof(hints));
    hints.ai_family = s->addrfamily;
    hints.ai_socktype = s->stype;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL,port,&hints,&res);

    p = res;
    while(p != NULL) {
        if((s->socket = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        if (setsockopt(s->socket, SOL_SOCKET, SO_REUSEADDR, &optval,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        if (bind(s->socket, p->ai_addr, p->ai_addrlen) == -1) {
            perror("server: bind");
            continue;
        }
        break;
        p= p->ai_next;
    }
    freeaddrinfo(res);

    while(1) {
        bzero(buffer,MAXBUFFER);
        size_t bytesread = recvfrom(s->socket,buffer,MAXBUFFER,0,(struct sockaddr *)&their_addr,(socklen_t*)&their_len);

        if(bytesread == -1) {
            perror("recvcfrom:");
            return -1;
        }
        int ret = 0;
        if((ret = c(strndup(buffer,bytesread),bytesread,jnx_socket_udp_resolve_ipaddress(their_addr))) != 0) {
            printf("Exiting udp listener with %d",ret);
            return 0;
        }
    }
    return -1;
}
