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

  typedef enum JNX_NETWORK_ENDIAN {
    JNX_LITTLE_ENDIAN,
    JNX_BIG_ENDIAN,
    JNX_UNKNOWN_ENDIAN
  }JNX_NETWORK_ENDIAN;
  /*
   * @fn jnx_network_get_endianness
   * @brief an enum that indicates the endien precedence on the system
   * @return an enum value of JNX_NETWORK_ENDIAN
   */
  JNX_NETWORK_ENDIAN jnx_network_get_endianness();

  /*
   * @fn int jnx_network_interface_to_string(char **obuffer,char *interface, unsigned int family)
   * @brief This will attempt to convert the chosen interface and family into a string representing the IP address
   * @param obuffer is pointer to the out buffer that will contained stored data
   * @param interface is the name of the interface to use (accepts const char *)
   * @param family is the family type to use - either AF_INET or AF_INET6
   * @return 0 on success 
   */
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

/**
 * @brief Basic HTTP Protocols 
 */
#define MAXBUFFER 2048
typedef enum JNX_HTTP_TYPE {
  JNX_HTTP_POST,
  JNX_HTTP_GET

} JNX_HTTP_TYPE;
typedef enum JNX_HTTP_STATE {
  JNX_HTTP_STATE_OKAY,
  JNX_HTTP_STATE_FAIL,
  JNX_HTTP_STATE_UNKNOWN
} JNX_HTTP_STATE;

size_t jnx_http_request(JNX_HTTP_TYPE type, const char *hostname, const char *page, char *args, uint8_t **out_reply,ssize_t *out_len);

JNX_HTTP_TYPE jnx_http_request_post(const char *hostname, const char *page, char *args,uint8_t **out_reply, ssize_t *out_len);

JNX_HTTP_TYPE jnx_http_request_get(const char *hostname, const char *page, char *args,uint8_t **out_reply, ssize_t *out_len);

#ifdef __cplusplus
}
#endif
#endif
