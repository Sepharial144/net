message(STATUS "Client sources: ${SOURCE_DEPENDENCIES}")
set(TARGET ${PROJECT_NAME}-client)

message(STATUS "Configure ${TARGET}")

set(CLIENT_EXAMPLE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/client.cpp")
add_executable(${TARGET} ${SOURCE_DEPENDENCIES} ${CLIENT_EXAMPLE_SRC})
target_link_libraries(${TARGET} PUBLIC net)

#target_link_libraries(${TARGET} PRIVATE net)
#target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/../../include/debug/lib")
#target_link_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/../../include/debug")

#include_directories("${CMAKE_SOURCE_DIR}/../../include/debug")