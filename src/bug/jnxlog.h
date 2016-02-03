/*********************************************************************************
 *     File Name           :     jnxlog.h
 *     Created By          :     tibbar
 *     Creation Date       :     [2015-05-14 14:01]
 *     Last Modified       :     [2016-02-03 08:55]
 *     Description         :      
 **********************************************************************************/
#ifndef __JNXLOG_H__
#define __JNXLOG_H__
#include "jnxtypes.h"
#ifdef __cplusplus
extern "C" {
#endif
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#define LDEBUG 0
#define LINFO 1
#define LWARN 2
#define LERROR 3
#define LPANIC 4
#define IS_INFO(X) (strcmp(X,"INFO")== 0)
#define IS_WARN(X) (strcmp(X,"WARN")== 0)
#define IS_ERROR(X) (strcmp(X,"ERROR")== 0)
#define IS_PANIC(X) (strcmp(X,"PANIC")== 0)
#define MAX_SIZE 2048
#define TIMEBUFFER 256
  static void jnx_log(jnx_int l, const jnx_char *file, 
      const jnx_char *function, 
      const jnx_uint32 line,const jnx_char *format,...) {
    jnx_char buffer[MAX_SIZE];
    jnx_char msgbuffer[MAX_SIZE];
    memset(buffer,0,MAX_SIZE);
    memset(msgbuffer,0,MAX_SIZE);
    va_list ap;
    va_start(ap,format);
    vsprintf(msgbuffer,format,ap);
    va_end(ap);
    time_t ptime;
    time(&ptime);
    jnx_char pbuffer[TIMEBUFFER];
    sprintf(pbuffer,"%s",ctime(&ptime));
    pbuffer[strlen(pbuffer)-1] = '\0';
    sprintf(buffer,"[%s][%s:%d][t:%s]%s\n",file,function,line,pbuffer,msgbuffer);
    printf(buffer);
  }

#define JNXLOG(LEVEL,FORMATTER, ...) jnx_log(LEVEL,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__);
#define JNX_LOG(LEVEL,FORMATTER, ...) jnx_log(LEVEL,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__);
#ifdef __cplusplus
}
#endif
#endif
