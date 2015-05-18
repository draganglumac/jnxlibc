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
int test_jnxnetwork(int args, char **argv) {
  JNXLOG(LDEBUG,"Running network tests...\n");
  JNXLOG(LDEBUG,"OK");
  JNXLOG(LDEBUG,"- displaying endianness\n");

  JNXLOG(LDEBUG,"BIG ENDIAN %s\n",jnx_network_get_endianness() == JNX_BIG_ENDIAN ? "YES" : "NO");
  JNXLOG(LDEBUG,"LITTLE ENDIAN %s\n",jnx_network_get_endianness() == JNX_LITTLE_ENDIAN ? "YES" : "NO");

  JNXLOG(LDEBUG,"OK");
  return 0;
}
