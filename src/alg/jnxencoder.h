/** @file jnxbase64.h
 * @brief The API for jnxlibc's implementation of Base-64 encoder and decoder.
 *
 * #include <jnxc_headers/jnxbase64.h>
 */
#ifndef __JNXENCODER_H__
#define __JNXENCODER_H__
#include <inttypes.h>
#ifdef __cplusplus
	extern "C" {
#endif

typedef struct jnx_encoder {
	char *b64_encoding_table;
	char *b64_decoding_table;
	int *b64_mod_table;
}jnx_encoder;

jnx_encoder* jnx_encoder_create();

void jnx_encoder_destroy(jnx_encoder **en);

uint8_t* jnx_encoder_b64_encode(jnx_encoder *e,uint8_t *data, size_t input_length, size_t *output_length);

uint8_t *jnx_encoder_b64_decode(jnx_encoder *e, uint8_t *data,size_t input_length, size_t *output_length);

#ifdef __cplusplus
	}
#endif
#endif
