set(TARGET ${PROJECT_NAME}-tcp-client)

set(DESTINATION_LIB "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}")

message(STATUS "Configure ${TARGET}")
message(STATUS "Path: ${DESTINATION_LIB}")
message(STATUS "Include dir: ${NET_INCLUDE_DIR}")

set(CLIENT_EXAMPLE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/tcp_client.cpp")
add_executable(${TARGET} ${SOURCE_DEPENDENCIES} ${CLIENT_EXAMPLE_SRC})
target_include_directories(${TARGET} PUBLIC "${DESTINATION_LIB}" "${NET_INCLUDE_DIR}")