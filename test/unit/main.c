/*
 * main.c
 * Copyright (C) 2015 tibbar <tibbar@tibbars-MacBook-Pro.local>
 *
 * Distributed under terms of the MIT license.
 */
#include "jnxlog.h"

int main(int argc, char **argv) {

  JNXLOG_CREATE("logger.conf");
  JNXLOG(LDEBUG,"---------------------TEST test_jnxbtree----------------------");
  test_jnxbtree();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxencoder--------------------");
  test_jnxencoder();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxexceptions-----------------");
  test_jnxexceptions();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxfile-----------------------");
  test_jnxfile();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxguid-----------------------");
  test_jnxguid();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxhash-----------------------");
  test_jnxhash();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxlist-----------------------");
  test_jnxlist();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxlog------------------------");
  test_jnxlog();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxnetwork--------------------");
  test_jnxnetwork();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxqueue----------------------");
  test_jnxqueue();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxquicksort------------------");
  test_jnxquicksort();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxsignal---------------------");
  test_jnxsignal();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxstack----------------------");
  test_jnxstack();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxstring---------------------");
  test_jnxstring();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxtcpsocket------------------");
  test_jnxtcpsocket();
  //JNXLOG(LDEBUG,"---------------------TEST test_jnxthread----------------------");
  //test_jnxthread();
  
  sleep(3);
  
  JNXLOG(LDEBUG,"---------------------TEST test_jnxudpsocket------------------");
  test_jnxudpsocket();
  //JNXLOG(LDEBUG,"---------------------TEST test_jnxunixsocket------------------");
  //test_jnxunixsocket();
  JNXLOG(LDEBUG,"---------------------TEST test_jnxvector---------------------");
  test_jnxvector();

  return 0;
}
