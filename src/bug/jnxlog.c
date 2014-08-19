/*
 * =====================================================================================
 *
 *       Filename:  jnxlog.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  18/08/2014 18:34:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include "jnxlog.h"
#include "jnxcheck.h"
#include "jnxfile.h"

jnx_log_config* jnx_log_create(const char *path,jnx_log_type output){  
  jnx_log_config *conf = malloc(sizeof(jnx_log_config));
  conf->disable_newline = 0;
  conf->output = output;
  time(&(conf->pstart));
  conf->pcurrent = 0;
  return conf;
}
size_t jnx_log(jnx_log_config *config, const char *file, const char *function,const int line,const char *format,...){
  JNXCHECK(config);
  JNXCHECK(file);
  JNXCHECK(function);
  JNXCHECK(format);
  size_t MAX_SIZE = 2048;
  char buffer[MAX_SIZE];
  char msgbuffer[MAX_SIZE];

  memset(msgbuffer,0,MAX_SIZE);
  va_list ap;
  va_start(ap,format);
  vsprintf(msgbuffer,format,ap);
  va_end(ap);

  memset(buffer,0,MAX_SIZE);
  sprintf(buffer,"[%s][%s:%d]%s",file,function,line,msgbuffer);
  switch(config->output) {
    case FILETYPE:
      JNXCHECK(config->log_path);
      jnx_file_write((char*)config->log_path ? (char*)config->log_path : "default.log",buffer,strlen(buffer),"w+");
      break;
    case CONSOLETYPE:
      printf(config->disable_newline ? "%s" : "%s\n",buffer);
      break;
  }
  time(&(config->pend));
  config->pcurrent = (config->pend - config->pstart);
  return strlen(buffer);
}
