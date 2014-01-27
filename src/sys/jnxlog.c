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
#include <pthread.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxmem.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
pthread_mutex_t _locker;
char *log_path = NULL;

void jnx_write_to_log(char *message)
{
#ifndef __JNX_LOG_SINGLE_THREAD__
	pthread_mutex_lock(&_locker);
#endif
	FILE *fp = fopen(log_path,"a");
    if(fp == NULL) { 
        printf("jnx_write_to_log: Unable to open file for log writing\n");
        pthread_mutex_unlock(&_locker);
        return;
    };
    fprintf(fp,"%s",(char*)message);
    fclose(fp);
    //free our string
#ifndef __JNX_LOG_SINGLE_THREAD__
    pthread_mutex_unlock(&_locker);
#endif
}
int jnx_log_setup(char *path)
{
    struct stat s;
    int err = stat(path,&s);
    if(-1 == err)
    {
        FILE *fp = fopen(path,"w+");
        if(fp == NULL)
        {
            printf("jnx_log_setup: Error creating log file\n");
            return 1;
        }
        else
        {
            fclose(fp);
            //after our successful log creation, we'll assign the log_path
            log_path = path;
        } 

    }else
    {
        if(S_ISDIR(s.st_mode))
        {
            printf("jnx_log_setup: Exists but is a directory\n");
            return 1;
        }
        printf("jnx_log_setup: Found existing log, will continue logging\n");
        //assigning to our global log path
        log_path = path;
    } 
    return 0;
}
char* jnx_get_time()
{
    time_t t;
    char *buf;
    time(&t);
    buf = (char*)malloc(strlen(ctime(&t)) +1);
    snprintf(buf,strlen(ctime(&t)),"%s",ctime(&t));
    return buf;
}
void jnx_log(const char * format, ...)
{
	if(log_path == NULL)
	{
		printf("jnx_log: Log path not set. See jnx_log_setup\n");
		return;
	}
	char output[MAX_LOG_SIZE];
    char buffer[MAX_ARG_SIZE];
    char *_time = jnx_get_time();
    strcpy(output,_time);
    strcat(output, ":");
    strcat(output, __FILE__);
    strcat(output, ": ");
    va_list ap;
    va_start(ap,format);
    vsprintf(buffer,format,ap);
    va_end(ap);
    strcat(output,buffer);
    free(_time);
#ifndef __JNX_LOG_SINGLE_THREAD__
	jnx_write_to_log(output);	
#else
	pthread_t _thr;
    pthread_create(&_thr,NULL,(void*)jnx_write_to_log,output);    
#endif
}

