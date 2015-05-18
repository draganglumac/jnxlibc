/*
 * main.c
 * Copyright (C) 2015 tibbar <tibbar@tibbars-MacBook-Pro.local>
 *
 * Distributed under terms of the MIT license.
 */
#include "jnxlog.h"

int main(int argc, char **argv) {

  JNXLOG_CREATE("logger.conf");
  test_jnxbtree();
  test_jnxencoder();
  test_jnxexceptions();
  test_jnxfile();
  test_jnxguid();
  test_jnxhash();
  test_jnxlist();
  test_jnxlog();
  test_jnxnetwork();
  test_jnxqueue();
  test_jnxquicksort();
  test_jnxsignal();
  test_jnxstack();
  test_jnxstring();
  test_jnxtcpsocket();
  test_jnxthread();
  test_jnxudpsocket();
  test_jnxunixsocket();
  test_jnxvector();
  return 0;
}
