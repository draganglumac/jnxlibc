IF (WIN32)
  set(PATH "win")
else()
  set(PATH "unix")
endif()

set(SOURCE ${SOURCE}
<<<<<<< HEAD
${CMAKE_CURRENT_SOURCE_DIR}/jnx_tcp_socket.c
${CMAKE_CURRENT_SOURCE_DIR}/jnx_udp_socket.c
${CMAKE_CURRENT_SOURCE_DIR}/jnx_network.c
${CMAKE_CURRENT_SOURCE_DIR}/jnx_socket.c
=======
${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnx_tcp_socket.c
${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnx_udp_socket.c
${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnxnetwork.c
${CMAKE_CURRENT_SOURCE_DIR}/${PATH}/jnxsocket.c
>>>>>>> upstream/pal
PARENT_SCOPE)

set(HEADERS
${HEADERS}
${CMAKE_CURRENT_SOURCE_DIR}/jnx_tcp_socket.h
${CMAKE_CURRENT_SOURCE_DIR}/jnx_udp_socket.h
${CMAKE_CURRENT_SOURCE_DIR}/jnx_network.h
${CMAKE_CURRENT_SOURCE_DIR}/jnx_socket.h
PARENT_SCOPE)

set(LINKER_LIBRARIES
  ${LINKER_LIBRARIES}
  PARENT_SCOPE)
