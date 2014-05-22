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
#ifdef __cplusplus
	}
#endif
#endif
