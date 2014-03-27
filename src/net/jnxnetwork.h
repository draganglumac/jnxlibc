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
#ifdef __cplusplus
	extern "C" {
#endif
#if defined(linux)
char *jnx_network_get_ip(char *interface,unsigned int addrfamily);
#endif
#ifdef __cplusplus
	}
#endif
#endif
