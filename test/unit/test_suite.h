#ifndef TEST_SUITE_H
#define TEST_SUITE_H

#define TEST_CASE(x) { #x, x }

extern int test_jnxbtree();
extern int test_jnxencoder();
extern int test_jnxexceptions();
extern int test_jnxfile();
extern int test_jnxguid();
extern int test_jnxhash();
extern int test_jnxipcsocket();
extern int test_jnxlist();
extern int test_jnxnetwork();
extern int test_jnxqueue();
extern int test_jnxquicksort();
extern int test_jnxsignal();
extern int test_jnxstack();
extern int test_jnxstring();
extern int test_jnxtcpsocket();
extern int test_jnxthread();
extern int test_jnxudpsocket();
extern int test_jnxvector();

typedef struct {
  char *test_case_name;
  int (*run_test_case)();
} test_case;

/*
 * WARNING - Keep test_suite array SORTED ALPHABETICALLY by test case name,
 * and remember to ADJUST test_suite_size to correct number of test cases
 * if you are adding or removing test cases to/from the suite.
 */
test_case test_suite[] = {
  TEST_CASE(test_jnxbtree),
  TEST_CASE(test_jnxencoder),
  TEST_CASE(test_jnxexceptions),
  TEST_CASE(test_jnxfile),
  TEST_CASE(test_jnxguid),
  TEST_CASE(test_jnxhash),
  TEST_CASE(test_jnxipcsocket),
  TEST_CASE(test_jnxlist),
  TEST_CASE(test_jnxnetwork),
  TEST_CASE(test_jnxqueue),
  TEST_CASE(test_jnxquicksort),
  TEST_CASE(test_jnxsignal),
  TEST_CASE(test_jnxstack),
  TEST_CASE(test_jnxstring),
  TEST_CASE(test_jnxtcpsocket),
  TEST_CASE(test_jnxthread),
  TEST_CASE(test_jnxudpsocket),
  TEST_CASE(test_jnxvector)
};

/*
 * The number of test cases in test_suite array - KEEP THIS CORRECT
 * otherwise test cases near the end might not get executed or worse
 * the program might crash if you have fewer test cases than test_suite_size.
 */
int test_suite_size = 18;

#endif // TEST_SUITE_H
