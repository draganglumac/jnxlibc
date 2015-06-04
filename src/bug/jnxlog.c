/*********************************************************************************
 *     File Name           :     /home/tibbar/Documents/logger/jnxlog.c
 *     Created By          :     tibbar
 *     Creation Date       :     [2015-05-14 14:08]
 *     Last Modified       :     [2015-06-04 07:48]
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
#include "jnxunixsocket.h"
#define MAX_SIZE 2048
#define TIMEBUFFER 256
#define IS_INFO(X) (strcmp(X,"INFO")== 0)
#define IS_WARN(X) (strcmp(X,"WARN")== 0)
#define IS_ERROR(X) (strcmp(X,"ERROR")== 0)
#define IS_PANIC(X) (strcmp(X,"PANIC")== 0)
#define IPC_PATH ".log_ipc_temp"
#define LOGLEVEL "LOG_LEVEL"
#define OUTPUTLOG "OUTPUT_LOG"

typedef struct jnx_log_conf {
  jnx_int level;
  jnx_int wfile;
  jnx_char *p;
  /* unix sock stream */
  jnx_unix_socket *unix_socket;
  jnx_unix_socket *unix_writer_socket;
  jnx_char initialized;
  jnx_char is_exiting;
  /* mutex */
  jnx_thread_mutex *locker;
  /* appender */
  jnx_log_appender appender;
}jnx_log_conf;

static jnx_log_conf _internal_jnx_log_conf = { 
  LDEBUG, 0, NULL, 0, 0
};
static void internal_appender_cli(jnx_char *message,jnx_size bytes_read){
  printf("%s",message);
}
static void internal_appender_io(jnx_char *message,jnx_size bytes_read){  
  jnx_thread_lock(_internal_jnx_log_conf.locker);
  jnx_file_write(_internal_jnx_log_conf.p,
      message,bytes_read,"a");
  jnx_thread_unlock(_internal_jnx_log_conf.locker);
}
static void internal_write_message(jnx_uint8 *buffer, jnx_size len) {
  jnx_unix_datagram_socket_send(_internal_jnx_log_conf.unix_writer_socket,buffer,len);
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
  if(IS_INFO(log_level)) { _internal_jnx_log_conf.level = LINFO;  }
  if(IS_WARN(log_level)) { _internal_jnx_log_conf.level = LWARN;  }
  if(IS_ERROR(log_level)){ _internal_jnx_log_conf.level = LERROR; }
  if(IS_PANIC(log_level)){ _internal_jnx_log_conf.level = LPANIC; }
}
void jnx_log_destroy() {
  _internal_jnx_log_conf.is_exiting = 1;
  if(_internal_jnx_log_conf.p) {
    free(_internal_jnx_log_conf.p);
  }
  jnx_unix_socket_destroy(&_internal_jnx_log_conf.unix_socket);
  jnx_unix_socket_destroy(&_internal_jnx_log_conf.unix_writer_socket);
  _internal_jnx_log_conf.initialized = 0;
}
static jnx_int internal_load_from_configuration(jnx_char *conf_path) {
  jnx_hashmap *h = jnx_file_read_kvp(conf_path,MAX_SIZE,"=");
  jnx_int is_valid = 1;
  if(h) {
    jnx_char *log_level = jnx_hash_get(h,LOGLEVEL);
    if(log_level) {
      internal_set_log_level(log_level);
      free(log_level);
    }else {
      is_valid = 0;
    }
    jnx_char *filepath = jnx_hash_get(h,OUTPUTLOG);
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
    jnx_hash_destroy(&h);
  }
  return is_valid;
}
static jnx_int32 internal_listener_callback(jnx_uint8 *payload, \
    jnx_size bytes_read, jnx_unix_socket *s) {
  jnx_char *buffer = strdup((jnx_char*)payload);
  _internal_jnx_log_conf.appender(buffer,strlen(buffer));
  free(buffer);
  return 0;
}
static void *internal_listener_loop() {
  jnx_unix_datagram_socket_listen(_internal_jnx_log_conf.unix_socket,
      internal_listener_callback);
  return NULL;
}
static void internal_load_listening_thread() {
  if(jnx_file_exists(IPC_PATH)) {
    remove(IPC_PATH);
  } 
  _internal_jnx_log_conf.unix_socket = 
    jnx_unix_datagram_socket_create(IPC_PATH);
  _internal_jnx_log_conf.unix_writer_socket = 
    jnx_unix_datagram_socket_create(IPC_PATH);
  jnx_thread_create_disposable(internal_listener_loop,NULL);
}
void jnx_log_create(jnx_char *conf_path) {
  if(_internal_jnx_log_conf.initialized) {
    JNXLOG(LWARN,"jnx_log_create: Logging has already been initialized");
    return;
  }
  if(!internal_load_from_configuration(conf_path)) {
    JNXLOG(LERROR,
        "jnx_log_create: Validation errors in internal_load_from_configuration");    
    return;
  }
  internal_load_listening_thread();
  switch(_internal_jnx_log_conf.wfile) {
    case 0:
      _internal_jnx_log_conf.appender = internal_appender_cli;
      break;
    case 1:
      _internal_jnx_log_conf.appender = internal_appender_io;
      break;
  }
  _internal_jnx_log_conf.initialized = 1;
}
void jnx_log_register_appender(jnx_log_appender ap) {
  JNXLOG(LDEBUG,"jnx_log_register_appender: Switching appender");
  _internal_jnx_log_conf.appender = ap;
}
