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
#include <sys/types.h>
#include <sys/stat.h>
#include "jnxlog.h"
#include "jnxfile.h"
#include "jnxterm.h"
int main(int args, char **argv) {
  
  JNXLOG_CREATE("logger.conf");
  JNXLOG(LDEBUG,"Hello from DEBUG");
  JNXLOG(LINFO,"Hello from INFO");
  JNXLOG(LWARN,"Hello from WARN");
  JNXLOG(LERROR,"Hello from ERROR");
  JNXLOG(LPANIC,"Hello from PANIC");
  return 0;
}
