/*
 * =====================================================================================
 *
 *       Filename:  test_jnxexceptions.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/19/2015 08:24:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxlog.h"
#include "jnxcheck.h"

#define FOO_EXCEPTION (1)
#define BAR_EXCEPTION (2)
#define BAZ_EXCEPTION (3)

int main(int argc, char** argv)
{
  JNX_LOG(NULL,"Running test_jnxexception tests");

  int ar[3] = { 1,2,3};
  int c;
  for(c=0;c<3;++c) {

    int current_exception = ar[c];
    jnx_try {
      jnx_throw( ar[c] );
    }
    jnx_catch( FOO_EXCEPTION ) {
      JNX_LOG(NULL,"First exception fired correctly");
      JNXCHECK(current_exception == FOO_EXCEPTION);
      break;
    }
    jnx_catch( BAR_EXCEPTION ) {
      JNX_LOG(NULL,"Second exception fired correctly");
      JNXCHECK(current_exception == BAR_EXCEPTION);
      break;
    }
    jnx_catch( BAZ_EXCEPTION ) {
      JNX_LOG(NULL,"Third exception fired correctly");
      JNXCHECK(current_exception == BAZ_EXCEPTION);
      break;
    }
    jnx_try_end;
  }
  return 0;
}
