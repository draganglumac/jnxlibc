/** @file jnxbase64.h
 * @brief The API for jnxlibc's implementation of Base-64 encoder and decoder.
 *
 * #include <jnxc_headers/jnxbase64.h>
 */
#ifndef __JNXBASE64_H__
#define __JNXBASE64_H__
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


char* jnx_encoder_b64_encode(jnx_encoder *e,const unsigned char *data, size_t input_length, size_t *output_length);

unsigned char *jnx_encoder_b64_decode(jnx_encoder *e, const char *data,size_t input_length, size_t *output_length);

/** 
 * @fn jnx_base64_encode(const unsigned char *data, size_t input_length, size_t *output_length)
 * @brief The function Base-64 encodes a byte array.
 *
 * @param data raw data for encoding 
 * @param input_length of raw data
 * @param output_length pass a pointer in to be filled with the encoded size
 *
 * @return char array with encoded string
 */
char *jnx_base64_encode(const unsigned char *data,
		size_t input_length,size_t *output_length);

/**
 * @fn jnx_base64_decode(const char *data, size_t input_length, size_t *output_length)
 * @brief The function decodes a Base-64 encoded byte array.
 *
 * @param data encoded data for decoding
 * @param input_length of data to decode
 * @param output_length pointer for retrieving the decoded buffer length
 * 
 * @return unsigned char * array of decoded string
 */
unsigned char *jnx_base64_decode(const char *data,
		size_t input_length,size_t *output_length);
/**
 * @fn jnx_base64_cleanup(void)
 * @brief The function resets internal structures.
 *
 */
void jnx_base64_cleanup(void);
#ifdef __cplusplus
	}
#endif
#endif
