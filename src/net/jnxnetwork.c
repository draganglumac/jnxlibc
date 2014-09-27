/*
 * =====================================================================================
 *
 *       Filename:  jnxnetwork.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/02/14 17:11:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (),
 *   Organization:
 *
 * =====================================================================================
 */
#define _GNU_SOURCE
#include <arpa/inet.h>
#include <assert.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "jnxnetwork.h"
#include "jnxlog.h"
#include "jnxsocket.h"
#include "jnxcheck.h"

char* internal_address_info( struct ifaddrs *ifa,unsigned int family){
  struct sockaddr_in *s4;
  struct sockaddr_in6 *s6;
  char buf[64];
  bzero(buf,64);
  if(family == ifa->ifa_addr->sa_family) {
    if(family == AF_INET) {
      s4 = (struct sockaddr_in *)(ifa->ifa_addr);
      if (NULL == inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr), buf, sizeof(buf))){
        JNX_LOG(DEFAULT_CONTEXT,"%s: inet_ntop failed!\n", ifa->ifa_name);
        return NULL;
      } else {
        JNX_LOG(DEFAULT_CONTEXT,"IPv4 addr %s: %s\n", ifa->ifa_name, buf);
        return strdup(buf);
      }
    }else {
      s6 = (struct sockaddr_in6 *)(ifa->ifa_addr);
      if (NULL == inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s6->sin6_addr), buf, sizeof(buf))) {
        JNX_LOG(DEFAULT_CONTEXT,"%s: inet_ntop failed!\n", ifa->ifa_name);
        return NULL;
      } else {
        JNX_LOG(DEFAULT_CONTEXT,"IPv6 addr %s: %s\n", ifa->ifa_name, buf);
        return strdup(buf);
      }
    }
  }
  return NULL;
}
JNX_NETWORK_ENDIAN jnx_network_get_endianness() {
  union { uint8_t c[4]; uint32_t i;} u; 
  u.i = 0x01020304; 
  if(0x04 == u.c[0]) { 
    return JNX_BIG_ENDIAN;
  }; 
  if (0x01 == u.c[0]) {	
    return JNX_LITTLE_ENDIAN;
  }; 
  return JNX_UNKNOWN_ENDIAN;
}
int jnx_network_interface_to_string(char **obuffer,char *interface, unsigned int family){
  JNXCHECK(interface);
  JNXCHECK(family);
  JNXCHECK(family == AF_INET || family == AF_INET6);
  struct ifaddrs *myaddrs, *ifa;
  int status;
  status = getifaddrs(&myaddrs);
  char *outaddr = NULL;
  *obuffer = NULL;
  if (status != 0){
    perror("getifaddrs failed!");
    return 1;
  }
  for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next){
    if (NULL == ifa->ifa_addr){
      continue;
    }
    if ((ifa->ifa_flags & IFF_UP) == 0) {
      continue;
    }
    if(strcmp(ifa->ifa_name,interface) == 0) {
      outaddr = internal_address_info(ifa,family);
      if(outaddr){
        *obuffer = outaddr;
      }
    }
  }
  freeifaddrs(myaddrs);
  return 0;
}
int jnx_network_hostname_to_ip(unsigned hint_family,char *host, char **out_ip,unsigned int *out_addrfamily) {
  JNXCHECK(hint_family);
  JNXCHECK(host);
  JNXCHECK(hint_family == AF_INET || hint_family == AF_INET6);
  *out_addrfamily = -1;
  *out_ip = NULL;
  struct addrinfo hints, *res;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags |= AI_CANONNAME;
  char addrstr[100];
  void *ptr;
  int errcode = getaddrinfo(host,NULL,&hints,&res);
  if(errcode != 0) {
    return 1;
  }
  while(res) {
    inet_ntop(res->ai_family,res->ai_addr->sa_data,addrstr,100);
    switch(res->ai_family) {
      case AF_INET:
        ptr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
        break;
      case AF_INET6:
        ptr = &((struct sockaddr_in6 *) res->ai_addr)->sin6_addr;
        break;
    }
    inet_ntop(res->ai_family,ptr,addrstr,100);
    *out_addrfamily = res->ai_family;
    if(hint_family != res->ai_family) {
      res = res->ai_next;
    }else {
      break;
    }
  }
  if(addrstr != NULL) {
    *out_ip = strndup(addrstr,strlen(addrstr));
  }
  return 0;
}
size_t jnx_http_request(JNX_HTTP_TYPE type, const char *hostname, const char *page, char *args, uint8_t **out_reply,ssize_t *out_len) {
  JNXCHECK(hostname);
  JNXCHECK(page);
  JNXCHECK(type == JNX_HTTP_POST || type == JNX_HTTP_GET);
  char *verb = NULL;
  char *out_ip;

  switch(type) {
    case JNX_HTTP_POST:
      verb = "POST";
      break;
    case JNX_HTTP_GET:
      verb = "GET";
      break;
  }
  JNXCHECK(verb);
  unsigned int out_family;
  int ret = jnx_network_hostname_to_ip(AF_INET,(char*)hostname,&out_ip,&out_family);
  JNXCHECK(ret == 0);
  jnx_socket *sock = jnx_socket_tcp_create(out_family);
  JNXCHECK(sock);
  char sendbuffer[2048];
  snprintf(sendbuffer,sizeof(sendbuffer), 
      "%s %s HTTP/1.0\r\n"
      "Host: %s\r\n"
      "Content-type: application/x-www-form-urlencoded\r\n"
      "Content-length: %zu\r\n\r\n"
      "%s\r\n",verb, page, hostname, strlen(args),args);

  ssize_t l = jnx_socket_tcp_send_with_receipt(sock,
      (char*)hostname,"80",(uint8_t*)sendbuffer,(ssize_t)strlen(sendbuffer),out_reply);
  *out_len = l;  
  if(!l) {
    return JNX_HTTP_STATE_UNKNOWN;
  }
  return JNX_HTTP_STATE_OKAY;
}
JNX_HTTP_TYPE jnx_http_request_post(const char *hostname, const char *page, char *args,uint8_t **out_reply, ssize_t *out_len) {
  uint8_t *reply;
  JNX_HTTP_TYPE t = jnx_http_request(JNX_HTTP_POST,hostname,page,args,&reply,out_len);
  *out_reply = reply;
  return t;
}
JNX_HTTP_TYPE jnx_http_request_get(const char *hostname, const char *page, char *args,uint8_t **out_reply, ssize_t *out_len) {
  uint8_t *reply;
  JNX_HTTP_TYPE t = jnx_http_request(JNX_HTTP_GET,hostname,page,args,&reply,out_len);
  *out_reply = reply;
  return t;
}
