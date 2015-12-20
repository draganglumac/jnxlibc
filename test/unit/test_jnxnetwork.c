/*
 * =====================================================================================
 *
 *       Filename:  test_jnxhash.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/27/13 16:20:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include "jnxnetwork.h"
#include "jnxlog.h"
#include "jnxterm.h"
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
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
#include "jnx_tcp_socket.h"
#include <inttypes.h>
void test_endian() {
  JNXLOG(LDEBUG,"- displaying endianness");

  JNXLOG(LDEBUG,"BIG ENDIAN %s",jnx_network_get_endianness() == JNX_BIG_ENDIAN ? "YES" : "NO");
  JNXLOG(LDEBUG,"LITTLE ENDIAN %s",jnx_network_get_endianness() == JNX_LITTLE_ENDIAN ? "YES" : "NO");
  JNXLOG(LDEBUG,"OK");
}
void test_ip() {
  JNXLOG(LDEBUG,"- testing ip detection!");
  jnx_char *buffer;
  jnx_network_interface_ip(&buffer,NULL,AF_INET);
  free(buffer);
  JNXLOG(LDEBUG,"=>%s",buffer);
  jnx_network_interface_ip(&buffer,NULL,AF_INET6);
  JNXLOG(LDEBUG,"=> %s",buffer);
  free(buffer);
  JNXLOG(LDEBUG,"OK");
}
int test_jnxnetwork() {
  JNXLOG(LDEBUG,"Running network tests...");
  test_endian();
  test_ip();
  JNXLOG(LDEBUG,"OK");
  return 0;
}
