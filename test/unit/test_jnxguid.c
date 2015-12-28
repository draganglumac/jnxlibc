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
#include <unistd.h>
#include "jnxlog.h"
#include "jnxguid.h"

void test_generate_guid() {
  jnx_guid g;
  jnx_guid_state s = jnx_guid_create(&g); 
  JNXCHECK(g.guid);  

  jnx_char *ostr;
  jnx_guid_to_string(&g,&ostr);
  JNXLOG(LDEBUG,"GUID -> %s",ostr);

}
void test_compare_guid() {

  jnx_guid g;
  jnx_guid_create(&g); 
  jnx_guid h;
  jnx_guid_create(&h); 

  JNXCHECK(jnx_guid_compare(&g,&h) == JNX_GUID_STATE_FAILURE);
  JNXCHECK(jnx_guid_compare(&g,&g) == JNX_GUID_STATE_SUCCESS);
  JNXCHECK(jnx_guid_compare_raw(g.guid,h.guid) == JNX_GUID_STATE_FAILURE);
  JNXCHECK(jnx_guid_compare_raw(g.guid,g.guid) == JNX_GUID_STATE_SUCCESS);

}
void test_string() {

  jnx_int i;
  for(i=0;i<5;++i) {
    jnx_guid g;
    jnx_guid_create(&g); 
    char *ostr;
    jnx_guid_to_string(&g,&ostr);
    JNXLOG(LDEBUG,"GUID A=>%s",ostr);
    jnx_guid f;
    jnx_guid_from_string(ostr,&f);
    char *ostrtwo;
    jnx_guid_to_string(&f,&ostrtwo);
    JNXLOG(LDEBUG,"GUID B=>%s",ostrtwo);
    JNXCHECK(jnx_guid_compare(&g,&f) == JNX_GUID_STATE_SUCCESS);
  }
}
int test_jnxguid() {
  JNXLOG(LDEBUG,"Running test for jnxguid");

  test_generate_guid();
  test_compare_guid();
  test_string();
  return 0;
}
