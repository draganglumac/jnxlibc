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
#if defined(linux)
#include <linux/if_link.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <errno.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <stdio.h>
#endif
#include <stdlib.h>
#include "jnxnetwork.h"
#include "jnxlog.h"
#if defined(linux)

char* show_address_info( struct ifaddrs *ifa,unsigned int family){
	struct sockaddr_in *s4;
	struct sockaddr_in6 *s6;
	char buf[64];
	bzero(buf,64);
	if(family == ifa->ifa_addr->sa_family) {
		if(family == AF_INET) {
			s4 = (struct sockaddr_in *)(ifa->ifa_addr);
			if (NULL == inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr), buf, sizeof(buf))){
				JNX_LOGC(JLOG_DEBUG,"%s: inet_ntop failed!\n", ifa->ifa_name);
				return NULL;
			} else {
				JNX_LOGC(JLOG_DEBUG,"IPv4 addr %s: %s\n", ifa->ifa_name, buf);
				return strdup(buf);
			}
		}else {
			s6 = (struct sockaddr_in6 *)(ifa->ifa_addr);
			if (NULL == inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s6->sin6_addr), buf, sizeof(buf))) {
				JNX_LOGC(JLOG_DEBUG,"%s: inet_ntop failed!\n", ifa->ifa_name);
				return NULL;
			} else {
				JNX_LOGC(JLOG_DEBUG,"IPv6 addr %s: %s\n", ifa->ifa_name, buf);
				return strdup(buf);
			}
		}
	}
	return NULL;
}
char* jnx_network_interface_to_string(char *interface, unsigned int family) {
	struct ifaddrs *myaddrs, *ifa;
	int status;
	status = getifaddrs(&myaddrs);
	char *outaddr = NULL;
	if (status != 0){
		perror("getifaddrs failed!");
		exit(1);
	}

	for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next){
		if (NULL == ifa->ifa_addr){
			continue;
		}
		if ((ifa->ifa_flags & IFF_UP) == 0) {
			continue;
		}
		if(strcmp(ifa->ifa_name,interface) == 0) {
			outaddr = show_address_info(ifa,family);
		}
	}
	freeifaddrs(myaddrs);
	return outaddr;
}
#endif
