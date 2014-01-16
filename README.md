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

DEBUG will enable memory management
RELEASE or undefined will optimise the library for production
```
gcc main.c -o -ljnxc -DDEBUG
````
