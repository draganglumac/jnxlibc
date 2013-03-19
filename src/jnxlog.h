/*
 * =====================================================================================
 *
 *       Filename:  jnx_log.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/20/13 10:15:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __JNX_LOG_H__
#define __JNX_LOG_H__
#include <stdarg.h>
typedef enum { LOGWNEWLINE,LOGNOCHANGE} logopt;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_log_setup
 *  Description:   Sets up a logger on the target filepath with options
 * =====================================================================================
 */
int jnx_log_setup(char *path,logopt l);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  jnx_log
 *  Description:  Logs a message to the logger path
 * =====================================================================================
 */
void jnx_log(char *message);

#endif
