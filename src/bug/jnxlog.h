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
#ifdef __cplusplus
extern "C" {
#endif
  typedef enum jnx_log_type {
    CONSOLETYPE,
    FILETYPE
  } jnx_log_type;

  typedef struct jnx_log_config{
    const char *log_path;
    jnx_log_type output;
    int disable_newline;
    time_t pstart;
    time_t pend;
    double pcurrent;
  } jnx_log_config;

  jnx_log_config* jnx_log_create(const char *path,jnx_log_type output);

  void jnx_log_destroy(jnx_log_config **config);

  size_t jnx_log(jnx_log_config *config, const char *file, const char *function,const int line,const char *format,...);

#define JNX_LOG_CONTEXT(X) \
  jnx_log_config *X

#define JNX_LOG(CONFIG,FORMATTER, ...) jnx_log(CONFIG,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__); 

#define JNX_LOG_CREATE(PATH,OUTPUT) jnx_log_create(PATH,OUTPUT);

#define JNX_LOG_DESTROY(X) jnx_log_destroy(X);

#define JNX_LOG_SWITCH_OUTPUT(CONFIG,LOG_TYPE) \
  CONFIG->output=LOG_TYPE;
#define JNX_LOG_SWITCH_LOG_PATH(CONFIG,PATH) \
  CONFIG->log_path=PATH;
#ifdef __cplusplus
}
#endif
#endif
