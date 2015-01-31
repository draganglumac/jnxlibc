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
  JNX_LOG(NULL,"GUID -> %s",ostr);

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
void test_string_guid() {

  jnx_guid g;
  jnx_guid_create(&g);

  jnx_char *a;
  jnx_guid_to_string(&g,&a);

  JNX_LOG(NULL,"Guid from string => %s",a);

  jnx_guid f;
  jnx_guid_from_string(a,&f);
  
  jnx_char *b;
  jnx_guid_to_string(&f,&b);
 
  JNX_LOG(NULL,"A [%s]",a);
  JNX_LOG(NULL,"B [%s]",b);
  
  free(a);
  free(b);
}
int main(int args, char **argv) {
  JNX_LOG(NULL,"Running test for jnxguid");

  test_generate_guid();
  test_compare_guid();
  test_string_guid();
  return 0;
}
