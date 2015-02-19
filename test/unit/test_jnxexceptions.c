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
  jnx_try {
    printf("In Try Statement\n");
    jnx_throw( BAR_EXCEPTION );
  }
  jnx_catch( FOO_EXCEPTION ) {
    
    return 1;
  }
  jnx_catch( BAR_EXCEPTION ) {
    JNX_LOG(NULL,"Correct exception caught");
    return 0;
  }
  jnx_catch( BAZ_EXCEPTION ) {
    
    return 1;
  }
  jnx_try_end;

  return 1;
}
