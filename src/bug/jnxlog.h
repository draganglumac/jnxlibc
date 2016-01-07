/*********************************************************************************
 *     File Name           :     jnxlog.h
 *     Created By          :     tibbar
 *     Creation Date       :     [2015-05-14 14:01]
 *     Last Modified       :     [2016-01-06 17:54]
 *     Description         :      
 **********************************************************************************/
#ifndef __JNXLOG_H__
#define __JNXLOG_H__
#include "jnxtypes.h"
#ifdef __cplusplus
extern "C" {
#endif

#define LDEBUG 0
#define LINFO 1
#define LWARN 2
#define LERROR 3
#define LPANIC 4

  /*
   *@fn void(*jnx_log_appender)(jnx_char *logmessage, jnx_size bytes_read)
   *@brief this is the functor pattern for a user to provide for custom appender
   *@param logmessage is the message to log
   *@param bytes_read is the byte count of the logmessage
   */
  typedef void(*jnx_log_appender)(jnx_char *logmessage,jnx_size bytes_read);
  /*
   *@fn jnx_log_create(jnx_char *conf_path)
   *@brief creates the global log from the parameters defined in the config file
   *@param conf_path is a path to a valid config file
   */
  void jnx_log_create(jnx_char *conf_path);

  /*
   *@fn jnx_log_destroy
   *@brief shuts down the global log and releases resources and listener
   */
  void jnx_log_destroy();
  /*
   *@fn jnx_log_register_appender(jnx_log_appender *ap)
   *@brief a function for users to register a custom appender with
   */
  void jnx_log_register_appender(jnx_log_appender ap);
  /*
   *@fn jnx_log(jnx_int l, const jnx_char *file, 
   const jnx_char *function, const jnx_uint32 line, const jnx_char  *format, ...)
   *@brief this function should not be called directly by the user
   */
  void jnx_log(jnx_int l, const jnx_char *file, 
      const jnx_char *function, 
      const jnx_uint32 line,const jnx_char *format,...);


#define JNXLOG_DESTROY() jnx_log_destroy();

#define JNXLOG_CREATE(X) jnx_log_create(X);

#define JNXLOG_REGISTER_APPENDER(X) jnx_log_register_appender(X);

#define JNXLOG(LEVEL,FORMATTER, ...) jnx_log(LEVEL,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__);

#define JNX_LOG(LEVEL,FORMATTER, ...) jnx_log(LEVEL,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__);
#ifdef __cplusplus
}
#endif
#endif
