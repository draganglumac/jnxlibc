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
#include <string.h>
#include "jnxcheck.h"
#include "jnxlist.h"
#include "jnxterm.h"
#include "jnxlog.h"
struct foo {
  int number;
} foo;

void test_list_creation() {
  JNX_LOG(NULL,"- test_list_creation");
  jnx_list *secondlist = jnx_list_create();
  JNXCHECK(secondlist != NULL);
  jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
  struct foo *f = malloc(sizeof(foo));
  f->number = 10;
  JNX_LOG(NULL,"- test_list_insertion");
  jnx_list_add_ts(secondlist,(void*)f);
  jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
  JNX_LOG(NULL,"- test_list_removal");
  struct foo *output = (struct foo*)jnx_list_remove_ts(&secondlist);
  JNXCHECK(output->number == 10);
  free(output);
  jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
  JNX_LOG(NULL,"- test_list_deletion");
  jnx_list_destroy(&secondlist);
  jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
void test_list_index() {
  JNX_LOG(NULL,"- test_list_sequence");
  jnx_list *j = jnx_list_create();
  int count = 6;
  char *ar[] = { "A", "B", "C", "D", "E", "F" };
  int y;
  for(y = 0; y < count; ++y) {
    jnx_list_add_ts(j,ar[y]);
  }
  int x;
  for(x = count - 1; x != 0; --x) {
    char *current = jnx_list_remove_ts(&j);
    JNXCHECK(strcmp(current,ar[x]) == 0);
  }
  jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
  jnx_list_destroy(&j);
  JNXCHECK(j == NULL);
}
void test_data_removal() {
  JNX_LOG(NULL,"- test_data_removal");
  char *test_string = "AABBCC";
  jnx_list *list = jnx_list_create();
  int count = 10;
  int x;
  for(x = 0; x < count; ++x) {
    char *dynamic_string = malloc(strlen(test_string));
    jnx_list_add_ts(list,dynamic_string);
  }
  char *ret;
  while((ret = jnx_list_remove_ts(&list)) != NULL) {
    free(ret);
  }
  jnx_list_destroy(&list);
  JNXCHECK(list == NULL);
  jnx_term_printf_in_color(JNX_COL_GREEN, " OK\n");
}
void test_list_tail() {
  jnx_list *l = jnx_list_create();

  int count = 3;
  char *ar[] = { "A", "B", "C" };
  int x;
  for(x = 0; x < count; ++x) {
    jnx_list_add_ts(l,ar[x]);
  }
  int c=count;
  while(l->tail != NULL) {

    JNXCHECK(strcmp((char*)l->tail->_data,ar[c-1]) == 0);
    --c;
    l->tail = l->tail->prev_node;
  }

  jnx_list_destroy(&l);
}
void test_removal_front() {
  JNX_LOG(NULL,"- test_removal_front\n");
  jnx_list *l = jnx_list_create();
  int count = 3;
  char *ar[] = { "A", "B", "C" };
  int x;
  for(x = 0; x < count; ++x) {
    jnx_list_add_ts(l,ar[x]);
  }
  JNXCHECK(l->counter == 3);
  void *data = jnx_list_remove_front_ts(&l);
  JNXCHECK(strcmp(data,"A") == 0);
  JNXCHECK(l->counter == 2);
}
int test_contains_comparator(void *A, void *B) {
  
  if(A == B) {
    return 1;
  }
  return 0;
}
void test_contains() {
  jnx_list *l = jnx_list_create();
  
  char *text = "Text";
  char *another = "Another";
  char *fake = "Fake";
  jnx_list_add_ts(l,text);
  jnx_list_add_ts(l,another);

  JNXCHECK(jnx_list_contains_ts(l,text,test_contains_comparator) == 1);
  JNXCHECK(jnx_list_contains_ts(l,another,test_contains_comparator) == 1);
  JNXCHECK(jnx_list_contains_ts(l,fake,test_contains_comparator) == 0);

}
int main(int args, char **argv) {
  JNX_LOG(NULL,"Running list tests...\n");
  test_list_creation();
  test_data_removal();
  test_list_index();
  test_list_tail();
  test_removal_front();
  test_contains();
  JNX_LOG(NULL,"List tests completed.\n");
  return 0;
}
