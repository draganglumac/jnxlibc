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
#include <stdio.h>
#include "jnxlog.h"
#ifdef __cplusplus
extern		"C" {
#endif
#define MAXSTACKFRAMES 20
  void jnxcheck_backtrace(void);

#if !defined(RELEASE)

#define JNXCHECK(X)\
  do \
  { \
    if(!(X)) {\
      JNX_LOG(NULL,"CHECK FAILED: >%s< [%s:%s:%d]\n",#X,__FILE__,__FUNCTION__,__LINE__); \
      jnxcheck_backtrace(); \
      exit(1);\
    } \
  } \
  while(0) \

#else
#define JNXCHECK(X)\
  do { (void)sizeof(X); } while(0)
#endif
#define JNXFAIL(X)\
  JNX_LOG(NULL,"CHECK FAILED WITH >%s<\n",#X); \
  JNXCHECK(0) 
#endif
#ifdef __cplusplus
}
#endif
