/**
 * @file jnxlog.h
 * @brief Simple logger that allows program output to be captured to file. It allows one log file per process only.
 *
 * #include <jnxc_headers/jnxlog.h>
 */
#ifndef __JNX_LOG_H__
#define __JNX_LOG_H__
#include <stdarg.h>
#ifdef __cplusplus
	extern "C" {
#endif
#define MAX_LOG_SIZE 2048
#define MAX_ARG_SIZE 1024
#ifdef __JNX_LOG_FORCE_SYNC___
#define __JNX_LOG_SINGLE_THREAD__
#endif
/**
 * @fn jnx_log_setup(char *path)
 * 
 * @brief Call this function to create/open the log file.
 * This function should be called at least once before jnx_log is called 
 * for the first time in the process.
 *
 * @param path location of log output
 *
 * @return returns 0 on succes and 1 on failure 
 */
int jnx_log_setup(char *path);

/**
 * @fn jnx_log(const char *format, ...)
 *
 * @brief Call this function to log a formatted string to your log file.
 *			You pass it a format string and optional arguments to match
 *			the format rules of printf.
 *
 * @param format Log formatted string, same format as printf.
 * @param ... optional arguments for insertion into formatted string
 */
void jnx_log(const char * format, ...);
#ifdef __cplusplus
	}
#endif
#endif
