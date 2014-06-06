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

/*
 * @fn jnx_encoder *jnx_encoder_create
 * @brief creates a jnx_encoder object
 * @return poiner to jnx_encoder object
 */
jnx_encoder* jnx_encoder_create();
/*
 * @fn jnx_encoder_destroy(jnx_encoder **en)
 * @brief destroys an encoder object
 * @param en is a pointer-to-pointer of the object to destroy
 *
 */
void jnx_encoder_destroy(jnx_encoder **en);
/*
 * @fn uint8_t *jnx_encoder_b64_encode(jnx_encoder *e, uint8_t *data,size_t input_length,size_t *output_length)
 * @brief this will encode a binary string to within a 64 character range
 * @param e the jnx_encoder object to use
 * @param data is the binary data for encoding
 * @param input_length is the total in data
 * @param output_length is a pointer that will be populated with total written data
 * @return a mallocated uint8_t pointer that will hold the data array
 */
uint8_t* jnx_encoder_b64_encode(jnx_encoder *e,uint8_t *data, size_t input_length, size_t *output_length);
/*
 * @fn uint8_t *jnx_encoder_b64_dencode(jnx_encoder *e, uint8_t *data,size_t input_length,size_t *output_length)
 * @brief this will decode a binary string to within a 64 character range
 * @param e the jnx_encoder object to use
 * @param data is the data for dceoding
 * @param input_length is the total in data
 * @param output_length is a pointer that will be populated with total written data
 * @return a mallocated uint8_t pointer that will hold the data array
 */
uint8_t *jnx_encoder_b64_decode(jnx_encoder *e, uint8_t *data,size_t input_length, size_t *output_length);

#ifdef __cplusplus
	}
#endif
#endif
