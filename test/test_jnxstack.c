#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../src/jnxstack.h"
#include "../src/jnxterm.h"

char *data = "ASDFJKL;";

void test_initialising_and_deleting_empty_stack()
{
    printf("- test_initialising_and_deleting_empty_stack:");
    jnx_stack *s = jnx_stack_init();
    assert(jnx_stack_is_empty(s));
    jnx_stack_delete(s);
    assert(jnx_stack_is_empty(s));
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
void test_push_to_empty_stack()
{
    printf("- test_push_to_empty_stack:");
    jnx_stack *s = jnx_stack_init();
    jnx_stack_push(s, (void *)(data));
    assert(strcmp(data, (char *)(s->top->_data)) == 0);
    assert(s->count == 1);
    jnx_stack_delete(s);
    assert(jnx_stack_is_empty(s));
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
void test_push_null_data()
{
    printf("- test_push_null_data:");
    jnx_stack *s = jnx_stack_init();
    jnx_stack_push(s, NULL);
    assert(jnx_stack_is_empty(s));
    assert(s->count == 0);
    jnx_stack_delete(s);
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
void test_pop_off_empty_stack()
{
    printf("- test_pop_off_empty_stack:");
    jnx_stack *s = jnx_stack_init();
    void *data = jnx_stack_pop(s);
    assert(data == NULL);
    assert(jnx_stack_is_empty(s));
    jnx_stack_delete(s);
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
void test_pop_off_the_stack()
{
    printf("- test_pop_off_the_stack:");
    jnx_stack *s = jnx_stack_init();
    int i;
    for ( i = 0; i < strlen(data); i++ )
    {
        jnx_stack_push(s, (void *)(data + i));
    }
    assert(s->count == 8);
    char *top = (char *)jnx_stack_pop(s);
    assert(strcmp(";", top) == 0);
    assert(s->count == 7);
    jnx_stack_pop(s);
    jnx_stack_pop(s);
    top = (char *)jnx_stack_pop(s);
    assert(s->count == 4);
    assert(strcmp("JKL;", top) == 0);
    jnx_stack_pop(s);
    jnx_stack_pop(s);
    jnx_stack_pop(s);
    top = (char *)jnx_stack_pop(s);
    assert(jnx_stack_is_empty(s));
    assert(strcmp(data, top) == 0);
    jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");    
}
int main()
{
    printf("Running stack tests...\n");
    test_initialising_and_deleting_empty_stack();
    test_push_to_empty_stack();
    test_push_null_data();
    test_pop_off_empty_stack();
    test_pop_off_the_stack();
    printf("Stack tests completed.\n");
    
    return 0;
}