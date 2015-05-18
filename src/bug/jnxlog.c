/*********************************************************************************
 *     File Name           :     /home/tibbar/Documents/logger/jnxlog.c
 *     Created By          :     tibbar
 *     Creation Date       :     [2015-05-14 14:08]
 *     Last Modified       :     [2015-05-15 15:54]
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
#include "jnxfile.h"
#include "jnxlog.h"
#define MAX_SIZE 2048
#define TIMEBUFFER 256

typedef struct jnx_log_conf {
  jnx_int level;
  jnx_int wfile;
  jnx_char *p;
}jnx_log_conf;

static jnx_log_conf _internal_jnx_log_conf = { LDEBUG, 0, NULL };

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
  if(l >= _internal_jnx_log_conf.level) {
    switch(_internal_jnx_log_conf.wfile) {
      case 0:
        printf("%s",buffer);
        break;
      case 1:
        jnx_file_write(_internal_jnx_log_conf.p,
            buffer,strlen(buffer),"a");
        break;
    }
  }
}
void internal_set_log_level(jnx_char *log_level) {
  if(strcmp(log_level,"INFO")== 0) {
    _internal_jnx_log_conf.level = LINFO;
  }
  if(strcmp(log_level,"WARN")== 0) {
    _internal_jnx_log_conf.level = LWARN;
  }
  if(strcmp(log_level,"ERROR")== 0) {
    _internal_jnx_log_conf.level = LERROR;
  }
  if(strcmp(log_level,"PANIC")== 0) {
    _internal_jnx_log_conf.level = LPANIC;
  }
}
void jnx_log_destroy() {
  if(_internal_jnx_log_conf.p) {
    free(_internal_jnx_log_conf.p);
  }  
}
void jnx_log_create(jnx_char *conf_path) {
  jnx_hashmap *h = jnx_file_read_kvp(conf_path,MAX_SIZE,"=");
  if(h) {
    JNXLOG(LDEBUG,"Reading from log conf file %s",conf_path);
    jnx_char *log_level = jnx_hash_get(h,"LOG_LEVEL");
    if(log_level) {
      internal_set_log_level(log_level);
      free(log_level);
    }
    jnx_char *filepath = jnx_hash_get(h,"OUTPUT_LOG");
    if(filepath) {
      _internal_jnx_log_conf.wfile = 1;
      jnx_char buffer[256] ={};
      time_t clk = time(NULL);
      jnx_char *t = ctime(&clk);
      t[strlen(t)-1] = '\0';
      sprintf(buffer,filepath,t);
      _internal_jnx_log_conf.p = strdup(buffer);
    }
    jnx_hash_destroy(&h);
  }
}
