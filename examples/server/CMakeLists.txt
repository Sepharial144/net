cmake_minimum_required (VERSION 3.23)

project(net VERSION 0.64 LANGUAGES CXX)

set(TARGET ${PROJECT_NAME}-server)

set(SERVER_EXAMPLE_SRC "server.cpp")
add_executable(${TARGET} ${SERVER_EXAMPLE_SRC})

target_link_libraries(${TARGET} PRIVATE net)
target_include_directories(${TARGET} PUBLIC "../../include/debug/net/lib")
target_link_directories(${TARGET} PUBLIC "../../include/debug")
