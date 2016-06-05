IF (WIN32)
  set(PATH "win")
else()
  set(PATH "unix")
endif()

set(SOURCE ${SOURCE}
<<<<<<< HEAD
${CMAKE_CURRENT_SOURCE_DIR}/jnx_string.c
${CMAKE_CURRENT_SOURCE_DIR}/jnx_term.c
=======
${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnxstring.c
${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnxterm.c
>>>>>>> upstream/pal
PARENT_SCOPE)

set(HEADERS
${HEADERS}
${CMAKE_CURRENT_SOURCE_DIR}/jnx_string.h
${CMAKE_CURRENT_SOURCE_DIR}/jnx_term.h
${CMAKE_CURRENT_SOURCE_DIR}/jnx_types.h
PARENT_SCOPE)

set(LINKER_LIBRARIES
  ${LINKER_LIBRARIES}
  PARENT_SCOPE)
