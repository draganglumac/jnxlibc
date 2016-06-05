IF (WIN32)
  set(PATH "win")
else()
  set(PATH "unix")
endif()

set(SOURCE ${SOURCE}
<<<<<<< HEAD
${CMAKE_CURRENT_SOURCE_DIR}/jnx_check.c
=======
  ${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnxcheck.c
>>>>>>> upstream/pal
PARENT_SCOPE)

set(HEADERS
${HEADERS}
${CMAKE_CURRENT_SOURCE_DIR}/jnx_check.h
${CMAKE_CURRENT_SOURCE_DIR}/jnx_log.h
PARENT_SCOPE)

set(LINKER_LIBRARIES
  ${LINKER_LIBRARIES}
  PARENT_SCOPE)
