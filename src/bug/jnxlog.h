/*********************************************************************************
 *     File Name           :     jnxlog.h
 *     Created By          :     tibbar
 *     Creation Date       :     [2015-05-14 14:01]
 *     Last Modified       :     [2016-02-03 15:09]
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
#define MAX_SIZE 2048
#define TIMEBUFFER 256
#ifndef RELEASE
  static void jnx_log(jnx_int l, const jnx_char *file, 
      const jnx_char *function, 
      const jnx_uint32 line,const jnx_char *format,...) {
    jnx_char levelb[128];
    bzero(levelb,128);
    switch(l) {
      case LDEBUG:
        strcpy(levelb,"DEBUG");
        break;
      case LINFO:
        strcpy(levelb,"INFO");
        break;
      case LWARN:
        strcpy(levelb,"WARN");
        break;
      case LERROR:
        strcpy(levelb,"ERROR");
        break;
      case LPANIC:
        strcpy(levelb,"PANIC");
        break;
    }
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
    sprintf(buffer,"[%s][%s][%s:%d][t:%s]%s\n",levelb,file,function,line,pbuffer,msgbuffer);
    printf(buffer);
  }
#define JNXLOG(LEVEL,FORMATTER, ...) jnx_log(LEVEL,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__);
#define JNX_LOG(LEVEL,FORMATTER, ...) jnx_log(LEVEL,__FILE__,__FUNCTION__,__LINE__,FORMATTER, ## __VA_ARGS__);
#else
#define JNXLOG(LEVEL,FORMATTER,...)
#define JNX_LOG(LEVEL,FORMATTER,...)
#endif
#ifdef __cplusplus
}
#endif
#endif
