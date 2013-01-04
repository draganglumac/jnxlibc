project(jnxlibc)

add_executable(testbay test/testbay.c src/jnxfile.c src/jnxstring.c)


add_subdirectory(src)
add_subdirectory(test)

