project(jnxlibc)

add_executable(jnxlibc test/testbay.c src/jnxfile.c)


add_subdirectory(src)
add_subdirectory(test)

