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
#if defined(WIN32)

#else
#include <arpa/inet.h>
#endif

#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#if defined(linux)
#include <linux/if_link.h>
#endif
#include <stdlib.h>
#include "jnxnetwork.h"
#if defined(linux)
char *jnx_network_get_ip(char *interface,unsigned int addrfamily) {
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];
    bzero(host,NI_MAXHOST);
    int n,s,family;
    if(getifaddrs(&ifaddr) != 0) {
        perror("getifaddrs:");
        return NULL;
    }
    for(ifa = ifaddr, n =0; ifa != NULL; ifa=ifa->ifa_next, ++n) {
        if(ifa->ifa_addr == NULL)
            continue;

        if(strcmp(ifa->ifa_name,interface) == 0) {
            family = ifa->ifa_addr->sa_family;
            if(family == addrfamily) {
                s = getnameinfo(ifa->ifa_addr,
                                (family == AF_INET) ?
                                sizeof(struct sockaddr_in) :
                                sizeof(struct sockaddr_in6),
                                host,
                                NI_MAXHOST,
                                NULL,
                                0,
                                NI_NUMERICHOST | NI_NUMERICSERV);
                if(s != 0) {
                    perror("getnameinfo:");
                    return NULL;
                }
                freeifaddrs(ifaddr);
                return strndup(host,strlen(host));
            }
        }
    }
    freeifaddrs(ifaddr);
    return NULL;
}
#endif
