/*
 * main.c
 * Copyright (C) 2015 tibbar <tibbar@tibbars-MacBook-Pro.local>
 *
 * Distributed under terms of the MIT license.
 */
#include "jnxlog.h"
#include <unistd.h>
#include <string.h>

typedef struct {
  char *test_case_name;

  void (*run_test_case)(int, char**);
} test_case;

void run_test_case(test_case tc) {
  char message[61];
  // format the log message by inserting test case name into formatted string
  strncpy(message,
          "--------------------TEST test_jnx---------------------------", 61);
  strncpy(message + 33, tc.test_case_name, strlen(tc.test_case_name));
  // actual run test case code starts here
  JNX_LOG(LDEBUG, message);
  tc.run_test_case();
}

/*
 * WARNING - Keep test_suite array SORTED ALPHABETICALLY by test case name,
 * and remember to ADJUST test_suite_size to correct number of test cases
 * if you are adding or removing test cases to/from the suite.
 */
test_case test_suite[] = {
  {"btree",      test_jnxbtree},
  {"encoder",    test_jnxencoder},
  {"exceptions", test_jnxexceptions},
  {"file",       test_jnxfile},
  {"guid",       test_jnxguid},
  {"hash",       test_jnxhash},
  {"list",       test_jnxlist},
  {"network",    test_jnxnetwork},
  {"queue",      test_jnxqueue},
  {"quicksort",  test_jnxquicksort},
  {"signal",     test_jnxsignal},
  {"stack",      test_jnxstack},
  {"string",     test_jnxstring},
  {"tcpsocket",  test_jnxtcpsocket},
  {"thread",     test_jnxthread},
  {"udpsocket",  test_jnxudpsocket},
  {"unixsocket", test_jnxunixsocket},
  {"vector",     test_jnxvector}
};

/*
 * The number of test cases in test_suite array - KEEP THIS CORRECT
 * otherwise test cases near the end might not get executed or worse
 * the program might crash if you have fewer test cases than test_suite_size.
 */
int test_suite_size = 18;

int compare_args(void *a1, void *a2) {
  char **s1 = (char **) a1;
  char **s2 = (char **) a2;
  return strcmp(*s1, *s2);
}

int main(int argc, char **argv) {

  JNXLOG_CREATE("logger.conf");
  sleep(3);
  sleep(3);

  if (argc > 2) { // sort argv[1] to argv[argc-1] alphabetically
    void *base = (void *)(argv + 1);
    qsort(base, argc - 1, sizeof(char *), compare_args);
  }

  if (argc > 1) {
    int next_tc = 1;
    for (int i = 0; i < test_suite_size; i++) {
      int cf_res = strcmp(test_suite[i].test_case_name, argv[next_tc]);

      if (cf_res == 0) {
        run_test_case(test_suite[i]);
        next_tc++;
      }
      else if (cf_res > 0)
        next_tc++;

      if (next_tc >= argc)
        break;
    }
  }
  else {
    for (int i = 0; i < test_suite_size; i++)
      run_test_case(test_suite[i]);
  }

  sleep(3);
  JNXLOG_DESTROY();
  return 0;
}
