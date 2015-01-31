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

jnx_int jnx_guid_get_fd() {
  return JNX_OPEN("/dev/urandom",O_RDONLY);
}
jnx_int jnx_guid_get_random_bytes(void *buf, jnx_size bytes) {

  jnx_int fd = jnx_guid_get_fd();
  if(fd < 0) {
    return -1;
  }
  jnx_int l=0;
  while(l< sizeof buf) {
    JNX_READ(fd,buf + l,bytes -l);
    ++l;
  }
  JNX_CLOSE(fd);
  return 0;
}

jnx_guid_state jnx_guid_create(jnx_guid *guid) {
  JNXCHECK(guid);
  if(jnx_guid_get_random_bytes(guid->guid,sizeof (guid->guid))) {
    return JNX_GUID_STATE_FAILURE;
  }
  JNXCHECK(sizeof(guid->guid) == 16);
  return JNX_GUID_STATE_SUCCESS;
}
jnx_guid_state jnx_guid_compare(jnx_guid *ga, jnx_guid *gb) {
  return jnx_guid_compare_raw(ga->guid,gb->guid);
}
jnx_guid_state jnx_guid_compare_raw(jnx_uint8 *ga, jnx_uint8 *gb) {
  jnx_int alen=0;
  while(alen<sizeof ga){
    if(ga[alen] != gb[alen]) {
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
  while(rlen< sizeof guid->guid) {
    jnx_char current[3]={};
    sprintf(current,"%02x",guid->guid[rlen]);
    strncat(obuffer,current,strlen(current));
    ++rlen;
  }
  *outstr = malloc(sizeof obuffer);
  memcpy(*outstr,obuffer,sizeof obuffer);
}
void jnx_guid_from_string(jnx_char *str, jnx_guid *oguid) {
  JNXCHECK(str);
  //rebase from base 16 into base 10 
  JNXCHECK(strlen(str) == 32);
  jnx_uint8 o[16];
  jnx_size c = 0;
  jnx_char *p = str;
  for(c=0;c<sizeof(o)/sizeof(o[0]);++c) {
    char buf[10];
    sprintf(buf,"%c%c",p[0],p[1]);
    o[c] = strtol(buf,NULL,10);
    p += 2 * sizeof(jnx_char);
  }
  memcpy(o,oguid->guid,16);
}
