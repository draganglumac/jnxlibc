/*********************************************************************************
 *     File Name           :     /home/tibbar/Documents/logger/jnxlog.c
 *     Created By          :     tibbar
 *     Creation Date       :     [2015-05-14 14:08]
 *     Last Modified       :     [2015-05-21 12:09]
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
#include "jnxnetwork.h"
#include "jnxthread.h"
#include "jnxlog.h"
#include "jnx_udp_socket.h"
#define MAX_SIZE 2048
#define TIMEBUFFER 256
#define IS_INFO(X) (strcmp(X,"INFO")== 0)
#define IS_WARN(X) (strcmp(X,"WARN")== 0)
#define IS_ERROR(X) (strcmp(X,"ERROR")== 0)
#define IS_PANIC(X) (strcmp(X,"PANIC")== 0)

typedef struct jnx_log_conf {
  jnx_int level;
  jnx_int wfile;
  jnx_char *p;
  /* listener */
  jnx_char *log_port;
  jnx_udp_listener *listener;
  /* writer */
  jnx_socket *writer;
  jnx_char initialized;
  jnx_char is_exiting;
  /* mutex */
  jnx_thread_mutex *locker;
}jnx_log_conf;

static jnx_log_conf _internal_jnx_log_conf = { 
  LDEBUG, 0, NULL, 0, 0
};

static void internal_write_message(jnx_uint8 *buffer, jnx_size len) {
  jnx_socket_udp_send(_internal_jnx_log_conf.writer,AF_INET4_LOCALHOST,
      _internal_jnx_log_conf.log_port,buffer,len);
}
void jnx_log(jnx_int l, const jnx_char *file, 
    const jnx_char *function, 
    const jnx_uint32 line,
    const jnx_char *format,...) {
  if(_internal_jnx_log_conf.initialized == 0) {
    printf("Logging system has not been correctly initialized. \
        \nPlease use 'JNXLOG_CREATE() to start\nCaught trying to log from file[%d]%s\n",line,file); 
      return; 
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
  sprintf(buffer,"[%s][%s:%d][t:%s]%s\n",file,function,line,pbuffer,msgbuffer);
  if(l >= _internal_jnx_log_conf.level) {
    internal_write_message((jnx_uint8*)buffer,strlen(buffer) + 1);   
  }
}
void internal_set_log_level(jnx_char *log_level) {
  if(IS_INFO(log_level)) {
    _internal_jnx_log_conf.level = LINFO;
  }
  if(IS_WARN(log_level)) {
    _internal_jnx_log_conf.level = LWARN;
  }
  if(IS_ERROR(log_level)) {
    _internal_jnx_log_conf.level = LERROR;
  }
  if(IS_PANIC(log_level)) {
    _internal_jnx_log_conf.level = LPANIC;
  }
}
void jnx_log_destroy() {
  _internal_jnx_log_conf.is_exiting = 1;
  if(_internal_jnx_log_conf.p) {
    free(_internal_jnx_log_conf.p);
  }  
  if(_internal_jnx_log_conf.log_port) {
    free(_internal_jnx_log_conf.log_port);
  }
  jnx_udp_listener *listener = _internal_jnx_log_conf.listener;
  jnx_socket *writer = _internal_jnx_log_conf.writer;
  jnx_socket_destroy(&writer);
  jnx_socket_udp_listener_destroy(&listener);
}
static void internal_load_from_configuration(jnx_char *conf_path) {
  jnx_hashmap *h = jnx_file_read_kvp(conf_path,MAX_SIZE,"=");
  if(h) {
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
      _internal_jnx_log_conf.locker = jnx_thread_mutex_create();
    }
    jnx_char *log_port = jnx_hash_get(h,"LOG_PORT");
    if(log_port) {
      _internal_jnx_log_conf.log_port = strdup(log_port);
      free(log_port);
    }else {
      printf("jnx_log_create: Log port must be set in the conf file e.g. LOG_PORT=9999\n");
    }
    jnx_hash_destroy(&h);
  }
}
static void internal_listener_callback(const jnx_uint8 *payload, \
    jnx_size bytes_read, void *args) {
  switch(_internal_jnx_log_conf.wfile) {
    case 0:
      printf("%s",payload);
      break;
    case 1:
      jnx_thread_lock(_internal_jnx_log_conf.locker);
      jnx_file_write(_internal_jnx_log_conf.p,
          (jnx_char *)payload,bytes_read,"a");
      jnx_thread_unlock(_internal_jnx_log_conf.locker);
      break;
  }
}
static void *internal_listener_loop() {
  while(!_internal_jnx_log_conf.is_exiting) {
    jnx_socket_udp_listener_tick(_internal_jnx_log_conf.listener,
        internal_listener_callback,NULL);
  }
  return NULL;
}
static void internal_load_listening_thread() {
  _internal_jnx_log_conf.writer = jnx_socket_udp_create(AF_INET);
  _internal_jnx_log_conf.listener = jnx_socket_udp_listener_create(
      _internal_jnx_log_conf.log_port,
      AF_INET);
  jnx_thread_create_disposable(internal_listener_loop,NULL);
}
void jnx_log_create(jnx_char *conf_path) {
  if(_internal_jnx_log_conf.initialized) {
    JNXLOG(LWARN,"Logging has already been initailized");
    return;
  }
  internal_load_from_configuration(conf_path);
  internal_load_listening_thread();
  _internal_jnx_log_conf.initialized = 1;
  JNXLOG(LDEBUG,"Logging service has started on port %s",_internal_jnx_log_conf.log_port);
}
