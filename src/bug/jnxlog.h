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

typedef enum JNX_LOG_LEVEL{ 
	JLOG_CRITICAL,
	JLOG_ALERT,
	JLOG_CAUTION,
	JLOG_NOTICE,
	JLOG_DEBUG,
	JLOG_NORMAL
} JNX_LOG_LEVEL;
#define JCRITICAL "CRITICAL"
#define JALERT "ALERT"
#define JCAUTION "CAUTION"
#define JNOTICE "NOTICE"
#define JDEBUG "DEBUG"
#define JNORMAL "NORMAL"
/**
 *@macro JNX_LOG_PATH
 *@brief set the log path
 */
#define JNX_LOG_PATH(X) jnx_log_set_path(X);
/**
 *@macro JNX_LOGC
 *@brief log to console
 */
#define JNX_LOGC(LEVEL,X, ...) jnx_log(LOG_CONSOLE,LEVEL,__FILE__,__FUNCTION__,__LINE__,X, ## __VA_ARGS__)		
/**
 *@macro JNX_LOGF
 *@brief log to file
 */
#define JNX_LOGF(LEVEL,X, ...) jnx_log(LOG_FILE,LEVEL,__FILE__,__FUNCTION__,__LINE__,X,  ## __VA_ARGS__)
		
/**
 * @fn jnx_log_set_path(char *path)
 * 
 * @brief Call this function to create/open the log file.
 * This function should be called at least once before jnx_log is called 
 * for the first time in the process.
 *
 * @param path location of log output
 *
 * @return returns 0 on succes and 1 on failure 
 */
int jnx_log_set_path(char *path);

/**
 *@fn jnx_log_get_path()
 *@brief gets the current log path if there is one set
 *@param path is a pointer to pointer that will be filled with log path on success
 *@return will return 1 on error
 */
int jnx_log_get_path(char **path);
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
typedef enum logtype { LOG_FILE, LOG_CONSOLE }logtype;
size_t jnx_log(const logtype l, JNX_LOG_LEVEL level, const char *file, const char *function,const int line,const char *format,...);

#ifdef __cplusplus
	}
#endif
#endif
