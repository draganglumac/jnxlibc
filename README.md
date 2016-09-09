
 jnxlibc
=======

[![Join the chat at https://gitter.im/AlexsJones/jnxlibc](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/AlexsJones/jnxlibc?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/AlexsJones/jnxlibc.svg?branch=master)](https://travis-ci.org/AlexsJones/jnxlibc)
[![License](http://img.shields.io/badge/license-GPLv3-green.svg)](http://img.shields.io/badge/license-GPLv3-green.svg)
[![Dev](http://img.shields.io/badge/development-active-green.svg)](http://img.shields.io/badge/development-active-green.svg)
![Issues](https://img.shields.io/github/issues/AlexsJones/jnxlibc.svg)
![Forks](https://img.shields.io/github/forks/AlexsJones/jnxlibc.svg)

>A modern C library that provides a useful self contained API that has built in testing and just works.
>Covering a range of boilerplate code areas and low level networking; jnxlibc provides both a reliable systems library and tool for learning.

[Latest release](https://github.com/AlexsJones/jnxlibc/releases)

[Latest iOS arm7 release](https://www.dropbox.com/sh/yrmpg4s54haobvk/AAACzIDXYKS7b7CZURzZb6Qaa?dl=0)

[Latest travis build](https://travis-ci.org/AlexsJones/jnxlibc/builds)


###Quick start
Requires: `gcc|llvm & cmake`
Paste this line into your terminal:
```
git clone https://github.com/AlexsJones/jnxlibc.git && cd jnxlibc && cmake . && make && sudo make install
```

#Key features
- Lightweight data structures with thread-safe API (`hash, list, vector, btree, stack, queue`) 
- `Socket` implmentation supporting multiple transport and `IPV4/IPV6 (Multicasting,Broadcast,Unicast) TCP/IP & UDP`
- `Network` utilities (get IP from hostname, resolve hostname from IP, interface to IP), multi-interface listeners, multiplexing server 
- `Threading` interface above POSIX (Ambition to extend x-platform)
- `Signals` & `Slots`
- `Interprocess communication`
- `Try/Catch/Finally` exception handling
- Built in assertions with Debug/Release flexibility
- Verbose terminal logging
- Easy to use `file read/write` functions
- Fast and easy `base64 encoding/decoding`
- Auto generated documentation and logical API
- Almost total test coverage (Unit tests/ Performance tests)
- Armv7 framework version available [Here](https://github.com/AlexsJones/jnxlibc_ios)

#Documentation & Examples

- First port of call: [Wiki](https://github.com/AlexsJones/jnxlibc/wiki/A-library-tour)
- MAN  docs/man
- HTML docs/html ([Here](http://htmlpreview.github.io/?https://raw.github.com/AlexsJones/jnxlibc/master/docs/html/files.html))
- [Loads of lovely tests in C!](test)
- Code reviewing

#Installation

###Dependencies
```
GCC
CMake
```
####Currently supported platforms: Linux distributions/OS X (Windows coming soon)


Default install location
```
git clone git@github.com:AlexsJones/jnxlibc jnxlibc && cd jnxlibc
cmake .   #or use cmake . -DRUN_TESTS=ON  #release on or off with -DRELEASE=OFF 
make
sudo make install
```
Custom install location
```
git clone git@github.com:AlexsJones/jnxlibc jnxlibc && cd jnxlibc
cmake . -DINSTALL_PREFIX:PATH=/Users/alexjo/Projects/jnxlibc/build/ 
make
sudo make install
```
> NOTE: You may want to use this option on OS X 10.11 El Capitan because you will not be able to install to the default location. You may want to use /usr/local as the INSTALL_PREFIX.

Use `-ljnxc` when building your project for shared library
Use `-ljnxcs` for static library

####Flags

```
cmake . -DRUN_TESTS=ON #To run tests at compile time
```

```
cmake . -DRELEASE #Sets library for release mode optimisations
```
An example command for a release build of the static library might be the following:
```
cmake . -DRUN_TESTS=ON -DRELEASE=ON
```
###Tests

To run tests from root of the project folder
```
./test/run_tests
```
To run an individual test
```
./test/run_tests test_jnxlist.c
```
##Examples
Listeners that can be manually ticked
```C
void test_tcp_listener_callback(jnx_uint8 *payload,
 jnx_size bytes_read, jnx_socket *s, jnx_int connected_socket,void *args){
 JNX_LOG(NULL,"test_tcp_listener_callback achieved");
 test_tcp_listener_complete = 1;
}
void test_tcp_listener() {
 jnx_tcp_listener *listener =
 jnx_socket_tcp_listener_create(TESTPORT,AF_INET,100);
 int x = 0;
 while(x < 5) {
  jnx_socket_tcp_listener_tick(listener,test_tcp_listener_callback,NULL);
   if(test_tcp_listener_complete)break;
  ++x;
}
 jnx_socket_tcp_listener_destroy(&listener);
 JNXCHECK(test_tcp_listener_complete);
 JNXCHECK(listener == NULL);
}

```
Sending message over network with exception handling.
```C
#include<jnxc_headers/jnx_udp_socket.h>

jnx_socket *udp_sock = jnx_socket_udp_create(AF_INET);
jnx_try {
 jnx_socket_udp_send(udp_sock,"host","port","message",strlen("message"));
} jnx_catch( NETWORK_EXCEPTION ) {
 JNX_LOG(DEFAULT_CONTEXT, "Failure in UDP network send!");
} jnx_finally {
 jnx_socket_close(udp_sock);
}
jnx_try_end
```
Using a binary tree
```C
jnx_btree *tree = jnx_btree_create(sizeof(int),callback_func);
//insertion
int i;
for(i=0;i<10;++i) {
  void *next = (void*)(guids[i]);
  jnx_btree_add(tree,next,next);
}
//get keys
jnx_list *keys = jnx_list_create();
jnx_btree_keys(tree,keys);

while(keys->head) {
  char *a_key = keys->head->data;
  void *val = jnx_btree_lookup(tree,(void*)a_key);
  jnx_btree_remove(tree,a_key,NULL,NULL);
  keys->head = keys->head->next_node;
}
jnx_list_destroy(&keys);
jnx_btree_destroy(&tree);
```
Manipulating data from a hashmap with thread safety
```C
void *value = jnx_hash_get_ts(hashmap,"Key");
jnx_hash_put_ts(hashmap,"Key",update_value);
```

Using signals and slots
```C

typedef void local_slot(jnx_signal *s);

jnx_signal *s = jnx_signal_create();
jnx_signal_connect(s,local_slot);
jnx_signal_call(s,NULL);
jnx_signal_destroy(&s);

```
