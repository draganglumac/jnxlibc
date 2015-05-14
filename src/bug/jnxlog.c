/*********************************************************************************
 *     File Name           :     /home/tibbar/Documents/logger/jnxlog.c
 *     Created By          :     tibbar
 *     Creation Date       :     [2015-05-14 14:08]
 *     Last Modified       :     [2015-05-14 14:56]
 *     Description         :      
 **********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include "jnxlog.h"

#define MAX_SIZE 2048
#define TIMEBUFFER 256
void jnx_log(jnx_int l, const jnx_char *file, 
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

#ifdef DEBUG
  printf("%s",buffer);
#endif
#ifdef INFO
  if(l == LDEBUG) return;
  printf("%s",buffer);
#endif
#ifdef WARN
  if(l == LDEBUG || l == LINFO) return;
  printf("%s",buffer);
#endif
#ifdef ERROR
  if(l == LDEBUG || l == LINFO || l == LWARN) return;
  printf("%s",buffer);
#endif
#ifdef PANIC
  if(l == LDEBUG || l == LINFO || l == LWARN || l == LERROR) return;
  printf("%s",buffer);
#endif
}
