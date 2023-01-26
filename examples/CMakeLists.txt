set(PROJECT_NAME net)

set(SERVER_EXAMPLE_SRC "server.cpp")
set(CLIENT_EXAMPLE_SRC "client.cpp")

add_executable(${PROJECT_NAME}-server ${SERVER_EXAMPLE_SRC})
target_include_directories(${PROJECT_NAME}-server PUBLIC "${CMAKE_SOURCE_DIR}/source")


add_executable(${PROJECT_NAME}-client ${CLIENT_EXAMPLE_SRC})
target_include_directories(${PROJECT_NAME}-client PUBLIC "${CMAKE_SOURCE_DIR}/source")