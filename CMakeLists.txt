cmake_minimum_required (VERSION 3.23)
#set(TARGET_NAME Net_server)
set(TARGET_NAME Net_client)

find_library(WSOCK32_LIBRARY wsock32.lib)
find_library(WS2_32_LIBRARY ws2_32.lib)
find_library(ADVAPI32_LIBRARY AdvApi32.lib)

file(GLOB_RECURSE HDR_FILES "source/*.hpp")
file(GLOB_RECURSE SRC_FILES TEST_SRC_FILES "source/*.cpp")

add_executable(${TARGET_NAME} ${HDR_FILES} ${SRC_FILES} "source/win32/net_server.cpp" "source/win32/net_client.cpp" "source/win32/net_connection.cpp" "source/common/common.cpp")
#add_executable(${TARGET_NAME} "source/main.cpp")

target_link_libraries(${TARGET_NAME} ${WSOCK32_LIBRARY} ${WS2_32_LIBRARY})
target_include_directories(${TARGET_NAME} PRIVATE "source")

add_subdirectory(examples)