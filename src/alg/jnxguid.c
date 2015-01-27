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
  jnx_int fd=JNX_OPEN("/dev/urandom",O_RDONLY);
  jnx_int rlen=0;
  while(rlen<sizeof guid->guid) {
    jnx_ssize r=JNX_READ(fd, guid->guid + rlen, (sizeof guid->guid) - rlen);
    if(rlen<=0) {
      return JNX_GUID_STATE_FAILURE;
    }
    rlen+=r;
  }
  JNX_CLOSE(fd);
  return JNX_GUID_STATE_SUCCESS;
}
jnx_guid_state jnx_guid_compare(jnx_guid *ga, jnx_guid *gb) {
  jnx_int alen=0;
  while(alen<sizeof ga->guid){
    if(ga->guid[alen] != gb->guid[alen]) {
      return JNX_GUID_STATE_FAILURE;
    }
    alen+=1;
  }
  return JNX_GUID_STATE_SUCCESS;
}
void jnx_guid_to_string(jnx_guid *guid,jnx_char **outstr) {
  JNXCHECK(guid);
  JNXCHECK(guid->guid);

  jnx_int rlen=0;
  jnx_char obuffer[33]={};
  while(rlen<sizeof guid->guid) {
    jnx_char current[3]; 
    sprintf(current,"%02x",guid->guid[rlen]);
    strncat(obuffer,current,4); 
    ++rlen;
  }

  *outstr = malloc(sizeof obuffer);
  memcpy(*outstr,obuffer,sizeof obuffer);
}
