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
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <sys/file.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxlist.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
pthread_mutex_t _locker;
char *log_path = NULL;
logopt _logoption;

void *jnx_write_to_log(char *message)
{
    pthread_mutex_lock(&_locker);
    FILE *fp = fopen(log_path,"a");
    if(fp == NULL) { 
        printf("Unable to open file for log writing\n");
        pthread_mutex_unlock(&_locker);
        exit(1);
    };
    switch(_logoption){
        case LOGWNEWLINE:
            fprintf(fp,"%s\n",(char*)message); 
            break;
        case LOGNOCHANGE:
            fprintf(fp,"%s",(char*)message);
            break;
        default:
            fprintf(fp,"%s",(char*)message);
    }
    fclose(fp);
    //free our string
    free(message);
    pthread_mutex_unlock(&_locker);
}
int jnx_log_setup(char *path,logopt l)
{
    if(l >= 2) { printf("Unknown log option\n"); return 1; };
    struct stat s;
    int err = stat(path,&s);
    if(-1 == err)
    {
        printf("No log file at %s\n",path);
        printf("Creating new log\n");
        FILE *fp = fopen(path,"w+");
        if(fp == NULL)
        {
            printf("Error creating log file\n");
            return 1;
        }
        else
        {
            printf("Done\n");
            fclose(fp);
            //after our successful log creation, we'll assign the log_path
            log_path = path;
        } 

    }else
    {
        if(S_ISDIR(s.st_mode))
        {
            printf("Exists but is a directory\n");
            return 1;
        }
        printf("Found existing log, will continue logging\n");
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
void jnx_log(char *message)
{
    char *output = (char*)malloc(256);
    char *timer = jnx_get_time();
    //to accomodate the extra colon we want to add to delimit time against message
    strcpy(output,timer);
    free(timer);
    strcat(output," : ");
    strcat(output,message);
    //the thread target function is responsible for freeing the string
    output = realloc(output,(strlen(output) + strlen(message)+1));
    pthread_t _thr;
    pthread_create(&_thr,NULL,(void*)jnx_write_to_log,output);    
}

