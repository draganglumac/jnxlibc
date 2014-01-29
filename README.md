jnxlibc
=======
[![Build Status](https://travis-ci.org/AlexsJones/jnxlibc.png)](https://travis-ci.org/AlexsJones/jnxlibc)

C based common code library based on libjnx philosophy of general purpose & lightweight development.

##Key features:
- Memory & stack tracing
- Simple listen/send over IP/port functions (Dynamic buffers for big messages!)
- Easy to use file read/write functions
- Fast and easy base64 encoding/decoding
- Simple logger system
- Terminal loading spinners and fancy colors done easily.
- Several useful data structures (hash, list, vector, btree)
- Auto generated documentation and logical API

#Installation

Currently supported platforms:
Linux distributions/OS X

Arch linux users
````
https://aur.archlinux.org/packages/jnxlibc/
````

Grab this repo.
````
//Test, Compile and install
./build_and_install.sh
or
Compile and install
./build_and_install.sh SKIP

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
int ret = jnx_network_send_message("10.0.0.1","9090","Message",strlen("Message"));
//Actually lets just broadcast that
int ret = jnx_network_broadcast("9090","255.255.255.0","Message");
```
Using memory management
```C
char *str = JNX_MEM_MALLOC(sizeof(char*));
size_t s = jnx_mem_get_current_size_allocations();
size_t cleared = jnx_mem_clear();
```

