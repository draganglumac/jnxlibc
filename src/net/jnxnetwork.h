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
int jnx_network_hostname_to_ip(unsigned hint_family,char *host, char **out_ip,unsigned int *out_addrfamily);
#ifdef __cplusplus
	}
#endif
#endif
