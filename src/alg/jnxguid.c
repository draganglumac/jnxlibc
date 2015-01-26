/*
 * =====================================================================================
 *
 *       Filename:  jnxguid.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26/01/2015 21:16:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com)
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxguid.h"
#include "jnxcheck.h"
jnx_guid_state jnx_guid_create(jnx_guid *guid) {
  JNXCHECK(guid);
  jnx_int fd = JNX_OPEN("/dev/urandom",O_RDONLY);
  jnx_int rlen = 0;
  while(rlen < sizeof guid->guid) {
    jnx_ssize r = JNX_READ(fd, guid->guid + rlen, (sizeof guid->guid) - rlen);
    if(rlen <= 0) {
      return JNX_GUID_STATE_FAILURE;
    }
    rlen += r;
  }
  JNX_CLOSE(fd);
  return JNX_GUID_STATE_SUCCESS;
}
void jnx_guid_to_string(jnx_guid *guid,jnx_char **outstr) {
  JNXCHECK(guid);
  JNXCHECK(guid->guid);
  *outstr = malloc(sizeof(jnx_char) * 17);
  bzero(*outstr,19);
  memcpy(*outstr,"{",1);
  memcpy(*outstr + 1,guid->guid,16);
  memcpy(*outstr + 17,"}",1);
}
