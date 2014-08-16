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
int main(int argc, char **argv) {

  JNX_LOGC(JLOG_DEBUG,"Running test for jnxencoder\n");

  jnx_encoder *e = jnx_encoder_create();

  char *test[] = {"A","BA","Some String","AbbA","A A A","Matey"};
  int x;

  for(x = 0; x < 5; ++x) {
    size_t test_len;

    char *binary_str;
    size_t blen = str_to_hex(test[x],&binary_str);
    char *encoded_test = jnx_encoder_b64_encode(e,binary_str,blen,&test_len);
    size_t decode_len;
    char *decoded_test = jnx_encoder_b64_decode(e,encoded_test,test_len,&decode_len);
    JNX_LOGC(JLOG_DEBUG,"Base64 encoding [%s] into hex --> [%s] into b64 --> [%s]\n",test[x],binary_str,encoded_test);
    JNXCHECK(strcmp(decoded_test,binary_str) == 0);
    free(encoded_test);

    free(binary_str);
  }
  jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");

  jnx_encoder_destroy(&e);
  JNXCHECK(e == NULL);
  return 0;
}
