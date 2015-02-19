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
int main(int args, char **argv) {
  JNX_LOG(NULL,"Running network tests...\n");
  jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
  JNX_LOG(NULL,"- displaying endianness\n");

  JNX_LOG(NULL,"BIG ENDIAN %s\n",jnx_network_get_endianness() == JNX_BIG_ENDIAN ? "YES" : "NO");
  JNX_LOG(NULL,"LITTLE ENDIAN %s\n",jnx_network_get_endianness() == JNX_LITTLE_ENDIAN ? "YES" : "NO");

  jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
  return 0;
}
