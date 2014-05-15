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
#include "jnxcheck.h"
#include "jnxthread.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
char* jnx_get_time() {
	time_t t;
	char *buf;
	time(&t);
	buf = (char*)malloc(strlen(ctime(&t)) +1);
	snprintf(buf,strlen(ctime(&t)),"%s",ctime(&t));
	return buf;
}
size_t jnx_log(JNX_LOG_LEVEL level, const char *file, const char *function,const int line,const char *format,...)
{    
	JNXCHECK(file);
	JNXCHECK(function);

	char output[MAX_LOG_SIZE];
	char buffer[MAX_ARG_SIZE];
	char *_time = jnx_get_time();
	char *warning_level = NULL;

	switch(level) {
		case JLOG_CRITICAL:
			warning_level = JCRITICAL;
			break;
		case JLOG_ALERT:
			warning_level = JALERT;
			break;
		case JLOG_CAUTION:
			warning_level = JCAUTION;
			break;
		case JLOG_NOTICE:
			warning_level = JNOTICE;
			break;
		case JLOG_DEBUG:
			warning_level = JDEBUG;
			break;
		case JLOG_NORMAL:
			warning_level = JNORMAL;
			break;
		default:
			warning_level = JNORMAL;
	}	

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
	strncat(output,"[",1);
	strncat(output,warning_level,strlen(warning_level));
	strncat(output,"]",1);
	va_list ap;
	va_start(ap,format);
	vsprintf(buffer,format,ap);
	va_end(ap);
	strcat(output,buffer);
	free(_time);
	size_t bytec = 0;
	bytec = strlen(output);
	
	printf("%s",output);
	return bytec;
}

