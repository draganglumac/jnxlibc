![jnx Icon](http://i.imgur.com/w6M1lJP.png) 
 jnxlibc
=======
[![Build Status](https://travis-ci.org/AlexsJones/jnxlibc.png)](https://travis-ci.org/AlexsJones/jnxlibc)

Status: Active development (Stable)

####Purpose

A modern C library that provides a useful self contained API that has built in testing and just works.
Covering a range of boilerplate code areas and low level networking; jnxlibc provides both a reliable systems library and tool for learning.

####For a solid snapshot of the library check out the releases
https://github.com/AlexsJones/jnxlibc/releases


#Key features
- Lightweight data structures with thread-safe API (hash, list, vector, btree, stack, queue) 
- Socket implmentation supporting multiple transport and IPV4/IPV6 (Multicasting,Broadcast,Unicast) TCP/IP & UDP
- Network utilities (get IP from hostname, resolve hostname from IP, interface to IP)
- Threading interface above POSIX (Ambition to extend x-platform)
- Signals & Slots
- Interprocess communication
- Verbose terminal logging
- Easy to use file read/write functions
- Fast and easy base64 encoding/decoding
- Auto generated documentation and logical API
- Almost total test coverage (Unit tests/ Performance tests)

#Documentation & Examples

- First port of call: [Wiki](https://github.com/AlexsJones/jnxlibc/wiki/A-library-tour)
- MAN  docs/man
- HTML docs/html ([Here](http://htmlpreview.github.io/?https://raw.github.com/AlexsJones/jnxlibc/master/docs/html/files.html))
- [Loads of lovely tests in C!](setup/test)
- Code reviewing

#Installation

###Dependencies
	- GCC
	- CMake
        - pthread library (included on OS/X and Linux)
Currently supported platforms:
Linux distributions/OS X

Arch linux users
````
yaourt jnxlibc 
````

Grab this repo.
````shell
cmake .
make
#Custom install path
#cmake . -DCMAKE_INSTALL_PREFIX=/Users/alexjo/Projects/jnxlibc/build/ 
sudo make install

Use -ljnxc when building your project for shared library
Use -ljnxcs for static library

````
####Flags

-DRELEASE (Sets library for release mode optimisations) 

##Examples

Sending message over network
```C
jnx_socket *udp_sock = jnx_socket_udp_create(AF_INET);
jnx_socket_udp_send(udp_sock,"host","port","message",strlen("message"));
jnx_socket_close(udp_sock);

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

