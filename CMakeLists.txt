cmake_minimum_required(VERSION 3.2)
set(INSTALL_PREFIX /usr CACHE PATH "installation root directory")
project(jnxlibc)

set(SOURCE "")
set(HEADERS "")

add_subdirectory(src/alg)
add_subdirectory(src/bug)
add_subdirectory(src/dat)
add_subdirectory(src/io)
add_subdirectory(src/ipc)
add_subdirectory(src/net)
add_subdirectory(src/sig)
add_subdirectory(src/sys)
add_subdirectory(src/thr)

include_directories(src/alg)
include_directories(src/bug)
include_directories(src/dat)
include_directories(src/io)
include_directories(src/ipc)
include_directories(src/net)
include_directories(src/sig)
include_directories(src/sys)
include_directories(src/thr)

file(MAKE_DIRECTORY jnxc_headers)
file(COPY ${HEADERS} DESTINATION jnxc_headers)

execute_process(
  OUTPUT_VARIABLE _version OUTPUT_STRIP_TRAILING_WHITESPACE
  COMMAND git rev-parse HEAD
  )
message(STATUS "Current library version ${_version}")
file(WRITE jnxc_headers/VERSION "${_version}")
file(WRITE jnxc_headers/jnx_version.h "#define VERSION=${_version}")

#Build and install function
function(build_install)
  option(OPT_DIR "Compile in optional module" OFF)
  if(OPT_DIR)
    message("Compiling optional module ${OPT_DIR}")
    add_subdirectory(src/${OPT_DIR})
    include_directories(src/${OPT_DIR})
  endif()

  option(RELEASE "Run in release mode" OFF)
  if(RELEASE)
    message("RELEASE")
    ADD_DEFINITIONS(-DRELEASE)
    ADD_DEFINITIONS(-DCMAKE_BUILD_TYPE=RELEASE)
  else()
    message("DEBUG")
    ADD_DEFINITIONS(-DDEBUG)
  endif()

  option(NO_IPV6_TESTS "Disabled tests that use IPv6" OFF)
  if(NO_IPV6_TESTS)
  endif()

  option(RUN_TESTS "Run jnxlibc tests" OFF)
  if(RUN_TESTS)
    option(NO_IPV6_TESTS "Disabled tests that use transport layer" OFF)
    if(NO_IPV6_TESTS)
      if(RELEASE)
        execute_process(COMMAND cmake . -DRELEASE=ON -DNO_IPV6_TESTS=ON WORKING_DIRECTORY test/unit)
      else()
        execute_process(COMMAND cmake . -DRELEASE=OFF -DNO_IPV6_TESTS=ON WORKING_DIRECTORY test/unit)
      endif()
    else()
      if(RELEASE)
        execute_process(COMMAND cmake . -DRELEASE=ON WORKING_DIRECTORY test/unit)
      else()
        execute_process(COMMAND cmake . -DRELEASE=OFF WORKING_DIRECTORY test/unit)
      endif()
    endif()
    execute_process(COMMAND make WORKING_DIRECTORY test/unit)
    execute_process(COMMAND ./test_run  WORKING_DIRECTORY test/unit RESULT_VARIABLE test_res)
    message("RET CODE IS => " ${test_res})
    if(${test_res} EQUAL 0)
      message("TESTS PASSED!")
    else()
      message(FATAL_ERROR "TESTS FAILED!")
    endif()
  endif()
  set(CMAKE_C_FLAGS "-Wall")
  set(CMAKE_C_FLAGS_DEBUG "-g -rdynamic")
  set(CMAKE_C_FLAGS_RELEASE "-O2")

  IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64") 
    ADD_DEFINITIONS(-fPIC) 
  ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")

  add_library(jnxcs STATIC ${SOURCE}) 
  add_library(jnxc SHARED ${SOURCE})

  target_link_libraries(jnxc ${LINKER_LIBRARIES})
  target_link_libraries(jnxcs ${LINKER_LIBRARIES})
  install(TARGETS jnxcs DESTINATION ${INSTALL_PREFIX}/lib)
  install(TARGETS jnxc DESTINATION ${INSTALL_PREFIX}/lib)
  install(DIRECTORY jnxc_headers DESTINATION ${INSTALL_PREFIX}/include)
endfunction()


#Compare versions
if(EXISTS "${INSTALL_PREFIX}/include/jnxc_headers/VERSION")
  file(STRINGS "${INSTALL_PREFIX}/include/jnxc_headers/VERSION" _found_version)
  message(STATUS "Existing library -- ${_found_version}")
  execute_process(
    OUTPUT_VARIABLE _diffresult
    COMMAND git diff ${_version} ${_found_version}
    )
  
  string(LENGTH "${_diffresult}" _len)

  if(${_len} GREATER 0) 
    message(STATUS "Found a differing library -- installing now!")
    build_install()
  else()
    message(STATUS "Identical library found -- ignoring!")
  endif()
else()
  message(STATUS "No existing library found -- installing now!")
  build_install()
endif()

