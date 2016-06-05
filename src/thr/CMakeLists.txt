IF (WIN32)
  set(PATH "win")
else()
  set(PATH "unix")
endif()

set(SOURCE ${SOURCE}
<<<<<<< HEAD
${CMAKE_CURRENT_SOURCE_DIR}/jnx_thread.c
=======
${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnxthread.c
>>>>>>> upstream/pal
PARENT_SCOPE)

set(HEADERS
${HEADERS}
${CMAKE_CURRENT_SOURCE_DIR}/jnx_thread.h
PARENT_SCOPE)

set(LINKER_LIBRARIES
  ${LINKER_LIBRARIES}
  pthread
  PARENT_SCOPE)
