/**
 * @file jnxlog.h
 * @brief Simple logger that allows program output to be captured to file
 */
#ifndef __JNX_LOG_H__
#define __JNX_LOG_H__
#include <stdarg.h>
/**
 * @fn jnx_log_setup(char *path)
 *
 * @param path location of log output
 *
 * @return success on 0
 */
int jnx_log_setup(char *path);

/**
 * @fn jnx_log(const char *format, ...)
 *
 * @param format Log formatted string
 * @param ... optional arguments for insertion into formatted string
 */
void jnx_log(const char * format, ...);

#endif
