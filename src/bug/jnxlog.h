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

typedef enum JNX_LOG_LEVEL{ 
	JLOG_CRITICAL,
	JLOG_ALERT,
	JLOG_CAUTION,
	JLOG_NOTICE,
	JLOG_DEBUG,
	JLOG_NORMAL
} JNX_LOG_LEVEL;
#define LOGTEMPLATE "[%s][%s][%s:%s:%d]"
#define JCRITICAL "CRITICAL"
#define JALERT "ALERT"
#define JCAUTION "CAUTION"
#define JNOTICE "NOTICE"
#define JDEBUG "DEBUG"
#define JNORMAL "NORMAL"
/**
 *@macro JNX_LOGC
 *@brief log to console
 */
#define JNX_LOGC(LEVEL,X, ...) jnx_log(LEVEL,__FILE__,__FUNCTION__,__LINE__,X, ## __VA_ARGS__)		
/**
 * @fn jnx_log(const char *format, ...)
 *
 * @brief Call this function to log a formatted string to your log file.
 *			You pass it a format string and optional arguments to match
 *			the format rules of printf.
 *
 * @param format Log formatted string, same format as printf.
 * @param ... optional arguments for insertion into formatted string
 * @return size_t of byte length of log entry
 */
size_t jnx_log(JNX_LOG_LEVEL level, const char *file, const char *function,const int line,const char *format,...);

#ifdef __cplusplus
	}
#endif
#endif
