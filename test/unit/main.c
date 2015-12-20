/*
 * main.c
 * Copyright (C) 2015 tibbar <tibbar@tibbars-MacBook-Pro.local>
 *
 * Distributed under terms of the MIT license.
 */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
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
