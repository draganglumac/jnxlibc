/*
 * main.c
 * Copyright (C) 2015 tibbar <tibbar@tibbars-MacBook-Pro.local>
 *
 * Distributed under terms of the MIT license.
 */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "jnxlog.h"
#include "test_suite.h"

void run_test_case(test_case tc) {
  char message[61];
  // format the log message by inserting test case name into formatted string
  strncpy(message,
          "--------------------TEST -----------------------------------", 61);
  strncpy(message + 25, tc.test_case_name, strlen(tc.test_case_name));
  // actual run test case code starts here
  JNX_LOG(LDEBUG, message);
  tc.run_test_case();
}

int compare_args(void *a1, void *a2) {
  char **s1 = (char **) a1;
  char **s2 = (char **) a2;
  return strcmp(*s1, *s2);
}

void print_usage() {
  printf("\n");
  printf("Usage\n");
  printf("\n");
  printf("\ttest_run [test_case1 test_case2 test_case3 ...]\n");
  printf("\ttest_run [--help]\n");
  printf("\n");
  printf("First usage example runs test cases test_case1 test_case2 test_case3 etc \n");
  printf("if they are specified as arguments. If NO ARGUMENTS are specified the whole\n");
  printf("test suite is run.\n");
  printf("\n");
  printf("The test_case arguments are short name for tests, so for example if you have\n");
  printf("a test files named test_jnxbtree.c and test_jnxvector.c, you would run them\n");
  printf("using the following command:\n");
  printf("\n");
  printf("\ttest_run btree vector\n");
  printf("\n");
  printf("Second usage example prints out this help on the console.\n");
  printf("\n");
}

int main(int argc, char **argv) {
  if (argc == 2 && strcmp(argv[1], "--help") == 0) {
    print_usage();
    return 0;
  }

  JNXLOG_CREATE("logger.conf");
  sleep(3);
  sleep(3);

  if (argc > 2) { // sort argv[1] to argv[argc-1] alphabetically
    void *base = (void *)(argv + 1);
    qsort(base, argc - 1, sizeof(char *), compare_args);
  }

  int i;
  if (argc > 1) {
    int next_tc = 1;
    for (i = 0; i < test_suite_size; i++) {
      int cf_res = strcmp(test_suite[i].test_case_name + 8, argv[next_tc]);

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
    for (i = 0; i < test_suite_size; i++)
      run_test_case(test_suite[i]);
  }

  sleep(3);
  JNXLOG_DESTROY();
  return 0;
}
