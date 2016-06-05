
IF (WIN32)
  set(PATH "win")
else()
  set(PATH "unix")
endif()

set(SOURCE ${SOURCE}
<<<<<<< HEAD
${CMAKE_CURRENT_SOURCE_DIR}/jnx_encoder.c
${CMAKE_CURRENT_SOURCE_DIR}/jnx_guid.c
${CMAKE_CURRENT_SOURCE_DIR}/jnx_quicksort.c
=======
  ${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnxencoder.c
  ${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnxguid.c
  ${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnxquicksort.c
>>>>>>> upstream/pal
PARENT_SCOPE)

set(HEADERS
${HEADERS}
${CMAKE_CURRENT_SOURCE_DIR}/jnx_encoder.h
${CMAKE_CURRENT_SOURCE_DIR}/jnx_guid.h
${CMAKE_CURRENT_SOURCE_DIR}/jnx_quicksort.h
PARENT_SCOPE)

set(LINKER_LIBRARIES
  ${LINKER_LIBRARIES}
  PARENT_SCOPE)
