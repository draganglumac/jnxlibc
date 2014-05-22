/*
 * =====================================================================================
 *
 *       Filename:  jnxnetwork.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/02/14 17:09:37
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __JNXNETWORK_H__
#define __JNXNETWORK_H__
#include <ifaddrs.h>
#include <inttypes.h>
#ifdef __cplusplus
	extern "C" {
#endif
int jnx_network_interface_to_string(char **obuffer,char *interface, unsigned int family);
/*
 * @fn int jnx_network_hostname_to_ip(unsigned hint_family,char *host, char **out_ip, unsigned int *out_addrfamily);
 * @brief This will attempt to use your hint for AF_INET|IAF_INET6 to resolve the ip address from hostname e.g. google.com
 * @param hint_family must be either AF_INET or AF_INET6
 * @param host is the ip string to use
 * @param out_ip is a pointer to string pointer you wish to have populated
 * @param out_addrfamily is a pointer to a pointer you wish to store the found address family
 * @return 0 on success
 */
int jnx_network_hostname_to_ip(unsigned hint_family,char *host, char **out_ip,unsigned int *out_addrfamily);
/*
 * @fn size_t jnx_network_request_POST(char *host, char *page, char *args, unsigned int addrfamily, uint8_t **out_reply);
 * @brief simple implementation of an http post on port 80
 * @param host is the host address to use
 * @param page is webpage to visit e.g. /home.html
 * @param args are the additional url arguments e.g. ?=listen.php
 * @param addrfamily is the connection method either AF_INET or AF_INET6
 * @param out_reply is a pointer to pointer to receive data
 * @return total readbytes
 */
size_t jnx_network_request_POST(char *host, char *page, char *args, unsigned int addrfamily,uint8_t **out_reply);

#ifdef __cplusplus
	}
#endif
#endif
