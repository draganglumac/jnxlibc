/*
 * =====================================================================================
 *
 *       Filename:  test_jnxhash.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/27/13 16:20:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../src/jnxhash.h"
int main(int args, char **argv)
{
    printf("Running test for jnxhash\n");
    
    //setup
    jnx_hashmap *testhash = jnx_hash_init(1024); 
     
    jnx_hash_put(testhash,"Alex","Jones");
  
    assert((char*)jnx_hash_get(testhash,"Alex") == "Jones"); 

    jnx_hash_delete(testhash);

    assert(testhash == NULL);
    return 0;
}
