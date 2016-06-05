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
 *         Author:  jonesax (jonesax@hush.com),
 *   Organization:
 *
 * =====================================================================================
 */
#define _GNU_SOURCE
#include <assert.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "jnxnetwork.h"
#include "jnxlog.h"
#include "jnx_tcp_socket.h"
#include "jnxcheck.h"
#include <winsock.h>

jnx_char* internal_address_info( struct ifaddrs *ifa,jnx_unsigned_int family){
 /* struct sockaddr_in *s4;
  struct sockaddr_in6 *s6;
  jnx_char buf[64];
  bzero(buf,64);
  if(family == ifa->ifa_addr->sa_family) {
    if(family == AF_INET) {
      s4 = (struct sockaddr_in *)(ifa->ifa_addr);
      if (NULL == inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr), 
            buf, sizeof(buf))){
        JNXLOG(LDEBUG,"%s: inet_ntop failed!\n", ifa->ifa_name);
        return NULL;
      } else {
        if(strcmp(buf,AF_INET4_LOCALHOST) == 0) {
          return NULL;
        }
        JNXLOG(LDEBUG,"IPv4 addr %s: %s\n", ifa->ifa_name, buf);
        return strdup(buf);
      }
    }else {
      s6 = (struct sockaddr_in6 *)(ifa->ifa_addr);
      if (NULL == inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s6->sin6_addr),
            buf, sizeof(buf))) {
        JNXLOG(LDEBUG,"%s: inet_ntop failed!\n", ifa->ifa_name);
        return NULL;
      } else {
        if(strcmp(buf,AF_INET6_LOCALHOST) == 0) {
          return NULL;
        }
        JNXLOG(LDEBUG,"IPv6 addr %s: %s\n", ifa->ifa_name, buf);
        return strdup(buf);
      }
    }
  }*/
  return NULL;
}
jnx_int32 jnx_network_interface_ip(jnx_char **obuffer, jnx_char *iface, 
	jnx_unsigned_int family){
 /* JNXCHECK(family);
  JNXCHECK(family == AF_INET || family == AF_INET6);
  if(!interface) {
    JNXLOG(LWARN,"No interface selected - Using default");
  }
  struct ifaddrs *myaddrs, *ifa;
  jnx_int32 status;
  status = getifaddrs(&myaddrs);
  jnx_char *outaddr = NULL;
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
    if(interface != NULL) {

      if(strcmp(ifa->ifa_name,interface) == 0) {
        outaddr = internal_address_info(ifa,family);
        if(outaddr){
          *obuffer = outaddr;
        }
      }
    }else {
      outaddr = internal_address_info(ifa,family);
        if(outaddr){
          *obuffer = outaddr;
          return 2;
        }
    }
  }
  freeifaddrs(myaddrs);*/
  return 0;
}
JNX_NETWORK_ENDIAN jnx_network_get_endianness() {
  union { 
    jnx_uint8 c[4]; 
    jnx_int32 i;
  } u; 
  u.i = 0x01020304; 
  if(0x04 == u.c[0]) { 
    return JNX_BIG_ENDIAN;
  }; 
  if (0x01 == u.c[0]) {	
    return JNX_LITTLE_ENDIAN;
  }; 
  return JNX_UNKNOWN_ENDIAN;
}
jnx_int32 jnx_network_hostname_to_ip(unsigned hint_family,
    jnx_char *host, jnx_char **out_ip,jnx_unsigned_int *out_addrfamily) {
 /* JNXCHECK(hint_family);
  JNXCHECK(host);
  JNXCHECK(hint_family == AF_INET || hint_family == AF_INET6);
  *out_addrfamily = -1;
  *out_ip = NULL;
  struct addrinfo hints, *res;
  memset(&hints,0,sizeof(hints));
  hints.ai_family = PF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags |= AI_CANONNAME;
  jnx_char addrstr[100]={};
  void *ptr;
  jnx_int32 errcode = getaddrinfo(host,NULL,&hints,&res);
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
  if(strlen(addrstr) > 0) {
    *out_ip = strndup(addrstr,strlen(addrstr));
  }*/
  return 0;
}
size_t jnx_http_request(JNX_HTTP_TYPE type, 
    const jnx_char *hostname, 
    const jnx_char *page, jnx_char *args, jnx_uint8 **out_reply,jnx_size *out_len) {
 /* JNXCHECK(hostname);
  JNXCHECK(page);
  JNXCHECK(type == JNX_HTTP_POST || type == JNX_HTTP_GET);
  jnx_char *verb = NULL;
  jnx_char *out_ip;

  switch(type) {
    case JNX_HTTP_POST:
      verb = "POST";
      break;
    case JNX_HTTP_GET:
      verb = "GET";
      break;
  }
  JNXCHECK(verb);
  jnx_unsigned_int out_family;
  jnx_int32 ret = jnx_network_hostname_to_ip(AF_INET,(jnx_char*)hostname,&out_ip,&out_family);
  JNXCHECK(ret == 0);
  jnx_socket *sock = jnx_socket_tcp_create(out_family);
  JNXCHECK(sock);
  jnx_char sendbuffer[2048];
  snprintf(sendbuffer,sizeof(sendbuffer), 
      "%s %s HTTP/1.0\r\n"
      "Host: %s\r\n"
      "Content-type: application/x-www-form-urlencoded\r\n"
      "Content-length: %zu\r\n\r\n"
      "%s\r\n",verb, page, hostname, strlen(args),args);

  jnx_size l = jnx_socket_tcp_send_with_receipt(sock,
      (jnx_char*)hostname,"80",(jnx_uint8*)sendbuffer,
      (jnx_size)strlen(sendbuffer),out_reply);
  *out_len = l;  
  if(!l) {
    return JNX_HTTP_STATE_UNKNOWN;
  }*/
  return JNX_HTTP_STATE_OKAY;
}
JNX_HTTP_TYPE jnx_http_request_post(const jnx_char *hostname, 
    const jnx_char *page, jnx_char *args,jnx_uint8 **out_reply, jnx_size *out_len) {
  jnx_uint8 *reply;
  JNX_HTTP_TYPE t = 
    jnx_http_request(JNX_HTTP_POST,hostname,page,args,&reply,out_len);
  *out_reply = reply;
  return t;
}
JNX_HTTP_TYPE jnx_http_request_get(const jnx_char *hostname, 
    const jnx_char *page, jnx_char *args,jnx_uint8 **out_reply, jnx_size *out_len) {
  jnx_uint8 *reply;
  JNX_HTTP_TYPE t = 
    jnx_http_request(JNX_HTTP_GET,hostname,page,args,&reply,out_len);
  *out_reply = reply;
  return t;
}
