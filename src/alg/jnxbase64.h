#ifndef __JNXBASE64_H__
#define __JNXBASE64_H__
#ifdef __cplusplus
	extern "C" {
#endif
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
		size_t input_length,
		size_t *output_length);

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
		size_t input_length,
		size_t *output_length);
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
