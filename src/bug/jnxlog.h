/*
 * =====================================================================================
 *
 *       Filename:  jnxlog.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  18/08/2014 18:23:34
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __JNXLOG_H__
#define __JNXLOG_H__
#include <time.h>
#include "jnxtypes.h"
#include "jnxthread.h"
#ifdef __cplusplus
extern "C" {
#endif
#define DEFAULT_CONTEXT NULL
  typedef enum jnx_log_type {
    CONSOLETYPE,
    FILETYPE
  } jnx_log_type;

  typedef struct jnx_log_config{
    const jnx_char *log_path;
    jnx_log_type output;
    struct timeval *pstart;
    struct timeval *pend;
    jnx_double pcurrent;
    jnx_thread_mutex *internal_lock;
  } jnx_log_config;

  /** @fn jnx_log_create(const jnx_char *path, jnx_log_type output)
   *  @brief creates a log context for the given logtype 
   *  @param path is the filepath to set for an output of FILETYPE, can be NULL if using CONSOLETYPE
   *  @param output defines the logger type, either FILETYPE or CONSOLETYPE
   *  @return jnx_log_config which is the current log context
   */
  jnx_log_config* jnx_log_create(const jnx_char *path,jnx_log_type output);
  /** @fn jnx_log_destroy(jnx_log_config **config)
   *  @brief destroys the current log context
   *  @param config is a pointer to pointer of current log context
   *  @warning passing a null object will trigger an assertion
   */
  void jnx_log_destroy(jnx_log_config **config);
  /** @fn jnx_log(jnx_log_config *config, const jnx_char *file, const jnx_char *function, const int line, const jnx_char *format,...)
   *  @brief logs a message to the context or defaults to stdout
   *  @param conifg this is the context to use for writing, can be NULL and will default to stdout
   *  @param file current file name [auto generated when using JNX_LOG macro]
   *  @param function name [auto generated when using JNX_LOG macro]
   *  @param line number [auto generated when using JNX_LOG macro]
   *  @param format is a const string for formatting the appended arguments
   *  @param ... and varadic args 
   */
  void jnx_log(jnx_log_config *config, const jnx_char *file, const jnx_char *function,const jnx_uint32 line,const jnx_char *format,...);

#define JNX_LOG_CONTEXT(X) \
  jnx_log_config *X

  /**@fn JNX_LOG(CONFIG,FORMATTER,...)
   * @brief a macro for logging that auto completes file,function,line information
   * @param CONFIG is the context to use, can be DEFAULT_CONTEXT or NULL
   * @param FORMATTER is the const jnx_char* formatter for displaying the subsequent args
   * @param ... any optional arguments to be rendered by the formatter
   */
#ifndef DEBUG
#define JNX_LOG(CONFIG,FORMATTER,...)
#else
#define JNX_LOG(CONFIG,FORMATTER, ...) jnx_log(CONFIG,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__); 
#endif
  /**@fn JNX_LOG_CREATE(PATH,OUTPUT)
   * @brief creates a log context
   * @param PATH is an optional value for log output path when using OUTPUT=FILETYPE, can be NULL
   * @param OUTPUT is the context output can be either FILETYPE or CONSOLETYPE, CONSOLETYPE can be NULL
   * @return JNX_LOG_CONTEXT which is a macro surrounding jnx_log_config
   */
#define JNX_LOG_CREATE(PATH,OUTPUT) jnx_log_create(PATH,OUTPUT);
  /**@fn JNX_LOG_DESTROY(X)
   * @brief destroys the provided log context
   * @param X is a pointer to pointer of log context 
   */
#define JNX_LOG_DESTROY(X) jnx_log_destroy(X);

#ifdef __cplusplus
}
#endif
#endif
