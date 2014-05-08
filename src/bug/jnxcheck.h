/*
 * =====================================================================================
 *
 *       Filename:  jnxcheck.h
 *
 *    Description:  This gives the option to disable assertions in release mode
 *
 *        Version:  1.0
 *        Created:  05/08/14 07:57:45
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  jonesax (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __JNX_CHECK_H__
#define __JNX_CHECK_H__
#include "jnxlog.h"

#define JNXCHECK(X)\
	do \
	{ \
		if(!(X)) {\
		JNX_LOGC(JLOG_CRITICAL,"CHECK FAILED: [%s:%s:%d]\n",__FILE__,__FUNCTION__,__LINE__); \
		} \
	} \
	while(0); \

#endif
