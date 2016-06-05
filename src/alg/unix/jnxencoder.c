/*
 * =====================================================================================
 *
 *       Filename:  jnxencoder.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/16/14 08:17:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (jonesax@hush.com), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <strings.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "jnxencoder.h"
#include "jnxcheck.h"
#include "jnxlog.h"
jnx_encoder* jnx_encoder_create() {

  char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
  };
  jnx_uint32 mod_table[] = {0, 2, 1};
  jnx_encoder *e = malloc(sizeof(jnx_encoder));
  e->b64_mod_table = calloc(3,sizeof(jnx_uint32));
  e->b64_encoding_table = calloc(65,sizeof(jnx_char));
  e->b64_decoding_table = calloc(256,sizeof(jnx_char));
  jnx_uint32 x,y;
  for(y=0;y<256;++y) {
    e->b64_decoding_table[y] = (jnx_char)-1;
  }	
  for(x=0;x<65;++x) {
    jnx_char current = encoding_table[x];
    e->b64_encoding_table[x] = current;
    e->b64_decoding_table[(unsigned char)e->b64_encoding_table[x]] = x;
  }
  for(x=0;x<3;++x) {
    e->b64_mod_table[x] = mod_table[x];
  }
  return e;
}
void jnx_encoder_destroy(jnx_encoder **en) {
  JNXCHECK(*en);
  free((*en)->b64_encoding_table);
  free((*en)->b64_decoding_table);
  free((*en)->b64_mod_table);
  free(*en);
  *en = NULL;
}
jnx_uint8* jnx_encoder_b64_encode(jnx_encoder *e,jnx_uint8 *data, size_t input_length, size_t *output_length) {
  JNXCHECK(data);
  JNXCHECK(input_length);
  *output_length = 4 * ((input_length + 2) / 3);
  jnx_uint8 *encoded_data = malloc(((*output_length) +1) * sizeof (jnx_char));
  if (encoded_data == NULL) return NULL;

  int i,j;

  for ( i = 0, j = 0; i < input_length;) {

    jnx_uint32 octet_a = i < input_length ? data[i] : 0;
    i++;
    jnx_uint32 octet_b = i < input_length ? data[i] : 0;
    i++;
    jnx_uint32 octet_c = i < input_length ? data[i] : 0;
    i++;
    jnx_uint32 triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

    encoded_data[j++] = e->b64_encoding_table[(triple >> 3 * 6) & 0x3F];
    encoded_data[j++] = e->b64_encoding_table[(triple >> 2 * 6) & 0x3F];
    encoded_data[j++] = e->b64_encoding_table[(triple >> 1 * 6) & 0x3F];
    encoded_data[j++] = e->b64_encoding_table[(triple >> 0 * 6) & 0x3F];
  }

  jnx_uint32 l;
  for(l = 0; l < e->b64_mod_table[input_length %3]; l++) {
    encoded_data[*output_length -1 -l] = '=';
  }
  encoded_data[*output_length ] = '\0';
  return encoded_data;
}

jnx_uint8 *jnx_encoder_b64_decode(jnx_encoder *e, jnx_uint8 *data,size_t input_length, size_t *output_length) {

  JNXCHECK(data);
  JNXCHECK(input_length);

  if (input_length % 4 != 0) return NULL;

  *output_length = input_length / 4 * 3;
  if (data[input_length - 1] == '=') (*output_length)--;
  if (data[input_length - 2] == '=') (*output_length)--;

  unsigned char *decoded_data = calloc(*output_length, sizeof(unsigned char));
  bzero(decoded_data,*output_length);
  if (decoded_data == NULL) return NULL;

  jnx_uint32 i,j;
  for (i = 0, j = 0; i < input_length;) {

    jnx_uint32 sextet_a = data[i] == '=' ? 0 & i++ : e->b64_decoding_table[(jnx_uint32)data[i++]];
    jnx_uint32 sextet_b = data[i] == '=' ? 0 & i++ : e->b64_decoding_table[(jnx_uint32)data[i++]];
    jnx_uint32 sextet_c = data[i] == '=' ? 0 & i++ : e->b64_decoding_table[(jnx_uint32)data[i++]];
    jnx_uint32 sextet_d = data[i] == '=' ? 0 & i++ : e->b64_decoding_table[(jnx_uint32)data[i++]];

    jnx_uint32 triple = (sextet_a << 3 * 6)
      + (sextet_b << 2 * 6)
      + (sextet_c << 1 * 6)
      + (sextet_d << 0 * 6);

    if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
    if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
    if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
  }
  return decoded_data;
}

