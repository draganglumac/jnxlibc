![jnx Icon](http://i.imgur.com/w6M1lJP.png) 
 jnxlibc
=======
[![Build Status](https://travis-ci.org/AlexsJones/jnxlibc.png)](https://travis-ci.org/AlexsJones/jnxlibc)

Test driven C library for reliable networking, data structures and utility functionality.
Constantly in development and refinement to make it as memory/thread safe as possible whilst maintaining
no dependencies and low foot print.

#Latest release: https://github.com/AlexsJones/jnxlibc/releases/tag/v1.5

#Key features:
- Threading
- Interprocess communication
- Lightweight data structures (hash, list, vector, btree, stack, queue) with thread safe API
- Network socket implmentation supporting multiple transport and IPV4/IPV6
- Easy to use file read/write functions
- Fast and easy base64 encoding/decoding
- Auto generated documentation and logical API

#Documentation & Examples

- MAN  docs/man
- HTML docs/html ([Here](http://htmlpreview.github.io/?https://raw.github.com/AlexsJones/jnxlibc/master/docs/html/files.html))
- Examples of functionality in test suite.


#Installation

Currently supported platforms:
Linux distributions/OS X

Arch linux users
````
yaourt jnxlibc 
````

Grab this repo.
````
./setup/build_and_install

Use -ljnxc when building your project

````
####Flags

-DRELEASE (Sets library for release mode optimisations) 

##Examples

Sending message over network
```C
jnx_socket *udp_sock = jnx_socket_udp_create(AF_INET);
jnx_socket_udp_send(udp_sock,"host","port","message",strlen("message"));
jnx_socket_close(tcp_sock);

jnx_socket *tcp_sock = jnx_socket_tcp_create(AF_INET6);
jnx_socket_tcp_send(tcp_sock,"host","port","message",strlen("message"));
jnx_socket_close(udp_sock);

```
Using a btree
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

