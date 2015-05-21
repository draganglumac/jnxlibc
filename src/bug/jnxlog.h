/*********************************************************************************
 *     File Name           :     jnxlog.h
 *     Created By          :     tibbar
 *     Creation Date       :     [2015-05-14 14:01]
 *     Last Modified       :     [2015-05-15 14:50]
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

  void jnx_log_create(jnx_char *conf_path);

  void jnx_log_destroy();
  
  void jnx_log(jnx_int l, const jnx_char *file, 
      const jnx_char *function, 
      const jnx_uint32 line,const jnx_char *format,...);

#define JNXLOG_DESTROY() jnx_log_destroy();

#define JNXLOG_CREATE(X) jnx_log_create(X);

#define JNXLOG(LEVEL,FORMATTER, ...) jnx_log(LEVEL,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__);

#define JNX_LOG(LEVEL,FORMATTER, ...) jnx_log(LEVEL,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__);
#ifdef __cplusplus
}
#endif
#endif
