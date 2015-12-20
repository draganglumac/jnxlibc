/*
 * =====================================================================================
 *
 *       Filename:  test_jnxbase64.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  07/10/13 13:39:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxencoder.h"
#include <string.h>
#include <stdio.h>
#include "jnxterm.h"
#include "jnxcheck.h"
#include "jnxlog.h"

size_t str_to_hex(char* s,char **out)
{
  char *buffer = calloc(strlen(s) +1,sizeof(char));
  memset(buffer,0,strlen(buffer) +1);
  char output[9];
  while (*s)
  {
    if(sprintf(output,"%02X",(unsigned)*s)) {
      strcat(buffer,output);
    }
    ++s;
  }
  *out = buffer;
  return strlen(buffer);
}
int test_jnxencoder() {

  JNXLOG(LDEBUG,"Running test for jnxencoder\n");

  jnx_encoder *e = jnx_encoder_create();

  char *test[] = {"A","BA","Some String","AbbA","A A A","Matey"};
  int x;

  for(x = 0; x < 5; ++x) {
    size_t test_len = strlen(test[x]) + 1;
    size_t otest_len;
    char *encoded_test = jnx_encoder_b64_encode(e,test[x],test_len,&otest_len);
    
    size_t decode_len;
    char *decoded_test = jnx_encoder_b64_decode(e,encoded_test,otest_len,&decode_len);
    
    
    JNXLOG(LDEBUG,"Base64 encoding [%s] into b64 --> [%s]",test[x], encoded_test);
    JNXLOG(LDEBUG,"Comparing [%s] and [%s]",decoded_test,test[x]);
    JNXCHECK(strcmp(decoded_test,test[x]) == 0);

  }
  JNXLOG(LDEBUG,"OK");

  jnx_encoder_destroy(&e);
  JNXCHECK(e == NULL);
  return 0;
}
