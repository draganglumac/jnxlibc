/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  09/09/13 10:19:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "jnxlog.h"
#include "jnxhash.h"
#include "jnxcheck.h"
#include "jnxterm.h"
void test_key_deletion() {
    jnx_hashmap *testhash = jnx_hash_create(1024);

    //push static test data
    jnx_hash_put(testhash,"Alex","Jones");

    char *value_data = malloc(56);
    strcpy(value_data,"Testi");
    //push dynamic test data
    jnx_hash_put(testhash,"Bob",value_data);
    //destroy statid value
    jnx_hash_delete_value(testhash,"Alex");
    //return pointer to dynamic data
    char *data_to_free = jnx_hash_delete_value(testhash,"Bob");
    //free data
    free(data_to_free);
    //assert the values are no longer available
    void *ret = jnx_hash_get(testhash,"Alex");
    void *ret2 = jnx_hash_get(testhash,"Bob");
    assert(ret == NULL);
    assert(ret2 == NULL);
    ///assert the map is empty
    const char **buffer;
    int num_keys = jnx_hash_get_keys(testhash,&buffer);
    assert(num_keys == 0);
    free(buffer);
    jnx_hash_destroy(&testhash);
	JNXCHECK(testhash == NULL);
}
void test_hash_get_keys() {
    jnx_hashmap *testhash = jnx_hash_create(1024);
    jnx_hash_put(testhash,"1","Data");
    jnx_hash_put(testhash,"2","Data");
    jnx_hash_put(testhash,"3","Data");
    jnx_hash_put(testhash,"4","Data");

    const char **buffer;
    int num_keys = jnx_hash_get_keys(testhash,&buffer);
    free(buffer);
    assert(num_keys == 4);
    jnx_hash_destroy(&testhash);
}
void test_hash_deletion() {
    jnx_hashmap *testhash = jnx_hash_create(1024);
    char *key_one = "ABC";
    char *key_two = "DEF";
    char *test_data = "Void";
    jnx_hash_put(testhash,key_one,test_data);
    jnx_hash_put(testhash,key_two,test_data);

    const char **keys;
    int num = jnx_hash_get_keys(testhash,&keys);
    int i;
    for(i=0; i<num; ++i) {
        char *value = jnx_hash_delete_value(testhash,keys[i]);
    }
    num = jnx_hash_get_keys(testhash,&keys);
    assert(num == 0);
    jnx_hash_destroy(&testhash);
}
int main(int argc, char **argv) {
    JNX_LOGC(JLOG_DEBUG,"Running test for jnxhash\n");
    JNX_LOGC(JLOG_DEBUG,"- test_key_deletion: ");
    test_key_deletion();
    jnx_term_printf_in_color(JNX_COL_GREEN,"OK\n");
    JNX_LOGC(JLOG_DEBUG,"- test_hash_deletion: ");
    test_hash_deletion();
    jnx_term_printf_in_color(JNX_COL_GREEN,"OK\n");
    JNX_LOGC(JLOG_DEBUG,"- test_hash_get_keys: ");
    test_hash_get_keys();
    jnx_term_printf_in_color(JNX_COL_GREEN,"OK\n");
    return 0;
}
