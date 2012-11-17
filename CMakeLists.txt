project(jnxlibc)

add_executable(jnxlibc test/testbay.c src/jnxnetwork.c src/jnxlist.c)


add_subdirectory(src)
add_subdirectory(test)

