/*
 * =====================================================================================
 *
 *       Filename:  jnx_log.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  02/20/13 10:17:02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (alexsimonjones@gmail.com),
 *   Organization:
 *
 * =====================================================================================
 */
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxmem.h"
#include "jnxthread.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
jnx_thread_mutex locker;
static char *log_path = NULL;

void *jnx_write_to_log(void *message) {
#if !defined( __JNX_LOG_SINGLE_THREAD__)
    jnx_thread_lock(&locker);
#endif
    FILE *fp = fopen(log_path,"a");
    if(fp == NULL) {
        printf("jnx_write_to_log: Unable to open file for log writing\n");
#if !defined( __JNX_LOG_SINGLE_THREAD__)
        jnx_thread_unlock(&locker);
#endif
        return (void*)1;
    };
    fprintf(fp,"%s",(char*)message);
    fclose(fp);
    //free our string
#if !defined( __JNX_LOG_SINGLE_THREAD__)
    jnx_thread_unlock(&locker);
#endif
    return 0;
}
int jnx_log_file_setup(char *path) {
    struct stat s;
    int err = stat(path,&s);
    if(-1 == err) {
        FILE *fp = fopen(path,"w+");
        if(fp == NULL) {
            printf("jnx_log_setup: Error creating log file\n");
            return 1;
        } else {
            fclose(fp);
            //after our successful log creation, we'll assign the log_path
            log_path = path;
        }

    } else {
        if(S_ISDIR(s.st_mode)) {
            printf("jnx_log_setup: Exists but is a directory\n");
            return 1;
        }
        printf("jnx_log_setup: Found existing log, will continue logging\n");
        //assigning to our global log path
        log_path = path;
    }
    return 0;
}
char* jnx_get_time() {
    time_t t;
    char *buf;
    time(&t);
    buf = (char*)malloc(strlen(ctime(&t)) +1);
    snprintf(buf,strlen(ctime(&t)),"%s",ctime(&t));
    return buf;
}
size_t jnx_log(const logtype l, const char *file, const char *function,const int line,const char *format,...) {
    char output[MAX_LOG_SIZE];
    char buffer[MAX_ARG_SIZE];
    char *_time = jnx_get_time();

    char lineconv[256];
    sprintf(lineconv,"%d",line);
    strcpy(output,"[");
    strncat(output,_time,strlen(_time));
    strncat(output,"]",1);
    strncat(output,"[",1);
    strncat(output,file,strlen(file));
    strncat(output, ":",1);
    strncat(output,function,strlen(function));
    strncat(output, ": ",1);
    strncat(output,lineconv,strlen(lineconv));
    strncat(output,"] ",2);
    va_list ap;
    va_start(ap,format);
    vsprintf(buffer,format,ap);
    va_end(ap);
    strcat(output,buffer);
    free(_time);
    size_t bytec = 0;
    bytec = strlen(output);
    switch(l) {
    case LOG_FILE:
        if(log_path == NULL) {
            printf("jnx_log: Log path not set. See jnx_log_setup\n");
            return 0;
        }
#if defined( __JNX_LOG_SINGLE_THREAD__)
        jnx_write_to_log(output);
#else
        jnx_thread_create_disposable(&jnx_write_to_log,output);
#endif
        break;
    case LOG_CONSOLE:
        printf(output);
        return bytec;
        break;
    }
    return 0;
}

