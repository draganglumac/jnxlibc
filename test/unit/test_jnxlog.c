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
#include <unistd.h>
#include "jnxlog.h"
#include "jnxfile.h"
#include "jnxterm.h"
#include "jnxtypes.h"

static jnx_int worker_complete = 0;
static jnx_int custom_appender_complete = 0;
static void custom_appender(jnx_char *message,jnx_size bytes_read){
  printf("%s",message);
  custom_appender_complete = 1;
}
static void *worker(void *args) {
  JNXLOG(LDEBUG,"Hello from DEBUG");
  JNXLOG(LINFO,"Hello from INFO");
  JNXLOG(LWARN,"Hello from WARN");
  JNXLOG(LERROR,"Hello from ERROR");
  JNXLOG(LPANIC,"Hello from PANIC");
  worker_complete = 1;
  return NULL;
}
int test_jnxlog() {

  JNXLOG_CREATE("logger.conf");

  jnx_thread_create_disposable(worker,NULL);
  JNXLOG(LDEBUG,"Hello from DEBUG");
  JNXLOG(LINFO,"Hello from INFO");
  JNXLOG_REGISTER_APPENDER(custom_appender);
  JNXLOG(LWARN,"Hello from WARN");
  JNXLOG(LERROR,"Hello from ERROR");
  JNXLOG(LPANIC,"Hello from PANIC");
  while(!worker_complete) {
    sleep(1);
  }
  JNXCHECK(custom_appender_complete == 1);
  return 0;
}
