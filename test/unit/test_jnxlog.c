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
#include <string.h>
#include "jnxfile.h"
#include "jnxlog.h"
#include "jnxterm.h"
int main(int args, char **argv) {
  JNX_LOGC(JLOG_DEBUG,"Running test for jnxlog");

  size_t bytes_out = jnx_log(JLOG_NORMAL,__FILE__,__FUNCTION__,__LINE__,"This is a log message\n");

  assert(bytes_out > 0);

  jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
  return 0;
}
