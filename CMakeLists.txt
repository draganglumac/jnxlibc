project(jnxlibc)

add_executable(jnxlibc test/testbay.c src/network.c src/list.c)


add_subdirectory(src)
add_subdirectory(test)