/*
 * =====================================================================================
 *
 *       Filename:  jnx_log.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/20/13 10:17:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (alexsimonjones@gmail.com), 
 *
 * =====================================================================================
 */

/**
 * @file jnxlog.h
 * @brief Simple logger that allows program output to be captured to file. It allows one log file per process only.
 *
 * #include <jnxc_headers/jnxlog.h>
 */
#ifndef __JNX_LOG_H__
#define __JNX_LOG_H__
#include <stdarg.h>
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
 * @brief Call this function to log a formatted string to your log file.i
 *
 * @param format Log formatted string, same format as printf.
 * @param ... optional arguments for insertion into formatted string
 */
void jnx_log(const char * format, ...);

#endif
