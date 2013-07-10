/*
 * =====================================================================================
 *
 *       Filename:  base64.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/25/13 16:52:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones, 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __JNXBASE64_H__
#define __JNXBASE64_H__

/** 
 * @fn jnx_base_encode(const unsigned char *data, size_t input_length, size_t *output_length)
 * 
 * @param data raw data for encoding 
 * @param input_length of raw data
 * @param output_length pass a pointer in to be filled with the encoded size
 * @return char array with encoded string
 */
char *jnx_base64_encode(const unsigned char *data,
		size_t input_length,
		size_t *output_length);

/**
 * @fn jnx_base_decode(const char *data, size_t input_length, size_t *output_length)
 *
 * @param data encoded data for decoding
 * @param input_length of data to decode
 * @param output_length pointer for retrieving the decoded buffer length
 * @return unsigned char * array of decoded string
 *
 */
unsigned char *jnx_base64_decode(const char *data,
		size_t input_length,
		size_t *output_length);
/**
 * @fn jnx_base64_cleanup(void)
 *
 */
void jnx_base64_cleanup(void);
#endif
