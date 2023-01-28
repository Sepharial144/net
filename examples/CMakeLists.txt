set(SERVER_EXAMPLE_SRC "server.cpp")
set(CLIENT_EXAMPLE_SRC "client.cpp")

set(TARGET ${PROJECT_NAME}-server)
add_executable(${TARGET} ${SERVER_EXAMPLE_SRC})
target_link_libraries(${TARGET} net)
target_link_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/include/lib")
target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/include")

set(TARGET ${PROJECT_NAME}-client)
add_executable(${TARGET} ${CLIENT_EXAMPLE_SRC})
target_link_libraries(${TARGET} net)
target_link_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/include/lib")
target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/include")