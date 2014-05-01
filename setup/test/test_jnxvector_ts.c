/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/16/13 11:42:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Alex Jones (), alexsimonjones@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "jnxvector.h"
#include <stdio.h>
#include <assert.h>
#include "jnxlog.h"
#include <time.h>
#include <string.h>
#include "jnxterm.h"
typedef struct {
    int a;
} teststruct;

void test_complex_insertion() {
    JNX_LOGC(JLOG_DEBUG,"- test_complex_insertion");
    jnx_vector *vector = jnx_vector_create();
    int spread[5] = { 1, 200, 412, 55, 65 };
    //we are testing to see how well the vector handles distribution and reallocation on demand
    int x;
    while(x < 5) {
        teststruct *temp = malloc(sizeof(teststruct));
        temp->a = x;
        jnx_vector_insert_at_ts(vector,spread[x],temp);
        teststruct *res = vector->vector[spread[x]]->data;
        assert(res->a == x);
        ++x;
    }
    jnx_vector_destroy(&vector);
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_sequential_insertion() {
    JNX_LOGC(JLOG_DEBUG,"- test_sequential_insertion");
    jnx_vector *vector = jnx_vector_create();

    int x = 0;
    do {
        jnx_vector_insert_ts(vector,x);
        ++x;
    } while(x < 100);
    int y = 0;
    while(y < vector->count) {
        assert(vector->vector[y]->data == y);
        ++y;
    }
    jnx_vector_destroy(&vector);
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_insert_position() {
    JNX_LOGC(JLOG_DEBUG,"- test_insert_position");
    jnx_vector *vector = jnx_vector_create();
    jnx_vector_insert_at_ts(vector,15,"Test");
    jnx_vector_insert_at_ts(vector,100,"Derp");
    assert(vector->vector[15]->used == 1);
    assert(vector->vector[100]->used == 1);
    assert((char*)vector->vector[15]->data == "Test");
    assert((char*)vector->vector[100]->data == "Derp");
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
void test_remove_position() {
    JNX_LOGC(JLOG_DEBUG,"- test_remove_position");
    jnx_vector *vector = jnx_vector_create();
    jnx_vector_insert_ts(vector,"Hello");
    jnx_vector_insert_at_ts(vector,90,"Bye");
    assert(vector->vector[15]->data == NULL);
    assert((char*)vector->vector[90]->data == "Bye");
    char *data = jnx_vector_remove_at_ts(vector,90);
    assert(strcmp("Bye",(char*)data) == 0);
    jnx_vector_destroy(&vector);
    jnx_term_printf_in_color(JNX_COL_GREEN, "  OK\n");
}
int main(int argc, char **argv) {
    JNX_LOGC(JLOG_DEBUG,"Running vector tests...\n");
    test_insert_position();
    test_remove_position();
    test_sequential_insertion();
    test_complex_insertion();
    JNX_LOGC(JLOG_DEBUG,"Vector tests completed.\n");
    return 0;
}
