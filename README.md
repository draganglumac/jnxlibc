jnxlibc
=======
[![Build Status](https://travis-ci.org/AlexsJones/jnxlibc.png)](https://travis-ci.org/AlexsJones/jnxlibc)

C based common code library based on libjnx philosophy of general purpose & lightweight development.
Built in the spirit of TDD ( [See Here](http://showterm.io/3dd2883f32e0e385be055#fast) )


##Key features:
- Threading
- Memory & stack tracing
- Network sockets for TCP/UDP send/listen
- Easy to use file read/write functions
- Fast and easy base64 encoding/decoding
- Robust logger system
- Terminal loading spinners and fancy colors done easily.
- Several useful data structures (hash, list, vector, btree)
- Auto generated documentation and logical API

#Documentation

- MAN  docs/man
- HTML docs/html ([Here](http://htmlpreview.github.io/?https://raw.github.com/AlexsJones/jnxlibc/master/docs/html/files.html))


#Installation

Currently supported platforms:
Linux distributions/OS X

Arch linux users
````
https://aur.archlinux.org/packages/jnxlibc/
````

Grab this repo.
````
//LINUX
./platform/linux/build_and_install

Use -ljnxc when building your project

````
###Flags

-DJNX_MEMORY_MANAGEMENT (Enables jnx memory management if JNX_MEM macros are being used)
-DRELEASE (Sets library for release mode optimisations) 
```
gcc ${YOUR_PROGRAM_FILES} -ljnxc 
````

###Examples

Creating a linkedlist
```C
jnx_list *exampleList = jnx_list_create(); 
jnx_list_add(exampleList,"A");
jnx_list_destroy(&exampleList);
```
Reading from a file
```C
char *buffer;
size_t readBytes = jnx_file_read("example.txt",&buffer,"r");
```
Sending TCP message over network
```C
jnx_socket *write_socket = jnx_network_socket_create(AF_INET,SOCK_STREAM);
size_t sent = jnx_network_send(write_socket,HOST,PORT,"Hello!",strlen("Hello!"));
```
Using memory management
```C
char *str = JNX_MEM_MALLOC(sizeof(char*));
size_t s = jnx_mem_get_current_size_allocations();
size_t cleared = jnx_mem_clear();
```

