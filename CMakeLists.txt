cmake_minimum_required (VERSION 3.20)

project(net VERSION 0.0.1 DESCRIPTION "net library" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

include(config.cmake)

file(GLOB_RECURSE HDR_FILES "source/*.hpp")
file(GLOB_RECURSE SRC_FILES "source/*.cpp")

if(WIN32)
    message(STATUS "Target os: win32")
    set(TARGET_OS "win")

    find_library(WSOCK32_LIBRARY wsock32.lib)
    find_library(WS2_32_LIBRARY ws2_32.lib)
    find_library(ADVAPI32_LIBRARY AdvApi32.lib)

    #message(STATUS "Library: ${WSOCK32_LIBRARY} ${WS2_32_LIBRARY} ${ADVAPI32_LIBRARY}" )

    set(LIB_DEPENDENCIES 
	    ${WSOCK32_LIBRARY} 
	    ${WS2_32_LIBRARY} 
	    ${ADVAPI32_LIBRARY}
    )

    list(FILTER HDR_FILES EXCLUDE REGEX ".linux")
    list(FILTER SRC_FILES EXCLUDE REGEX ".linux")

    set(NET_INCLUDE_DIR 
        "${CMAKE_CURRENT_SOURCE_DIR}/source" 
        "${CMAKE_CURRENT_SOURCE_DIR}/source/win32"
    )
elseif(UNIX)
    message(STATUS "Target os: linux")
    set(TARGET_OS "linux")

    list(FILTER HDR_FILES EXCLUDE REGEX ".win32")
    list(FILTER SRC_FILES EXCLUDE REGEX ".win32")

    set(NET_INCLUDE_DIR 
        "${CMAKE_CURRENT_SOURCE_DIR}/source" 
        "${CMAKE_CURRENT_SOURCE_DIR}/source/linux"
    )

    set(HDR_PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/source/net.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/net_api.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/source/linux_definitions.hpp"
    )
endif()

set(SOURCE_DEPENDENCIES ${HDR_FILES} ${SRC_FILES})

message(STATUS "Dependencies: ${SOURCE_DEPENDENCIES}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(LIB_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

set(LIBRARY_NAME ${PROJECT_NAME}xx)

add_library(${LIBRARY_NAME} STATIC ${SOURCE_DEPENDENCIES})

set_target_properties(${LIBRARY_NAME} 
    PROPERTIES 
    PUBLIC_HEADER ${HDR_PUBLIC})

target_link_libraries(${LIBRARY_NAME} ${LIB_DEPENDENCIES})

target_include_directories(${LIBRARY_NAME} PRIVATE ${NET_INCLUDE_DIR})

message(STATUS "INSTALL PREFIX: ${CMAKE_INSTALL_PREFIX}")

if(NET_TESTS)
    add_subdirectory(tests)
endif()

if (NET_EXAMPLES)
    if(WIN32)
        add_subdirectory("examples/client")
        add_subdirectory("examples/server")
        add_subdirectory("examples/${TARGET_OS}/async_tcp_client")
    endif()

    if(UNIX)
        add_subdirectory("examples/${TARGET_OS}/tcp/block/server")
        add_subdirectory("examples/${TARGET_OS}/tcp/block/client")
        add_subdirectory("examples/${TARGET_OS}/tcp/nonblock/server")
    endif()
endif()

# TODO: finish configure project
# fix execution install folder, does not work
message(STATUS "Build path: ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

install(TARGETS ${LIBRARY_NAME}
        CONFIGURATIONS Debug
        ARCHIVE DESTINATION "debug/${LIBRARY_NAME}/lib"
        RUNTIME DESTINATION "debug/${LIBRARY_NAME}/bin"
        PUBLIC_HEADER DESTINATION "debug/${LIBRARY_NAME}")
install(TARGETS ${LIBRARY_NAME}
        CONFIGURATIONS Release
        ARCHIVE DESTINATION "release/${LIBRARY_NAME}/lib"
        RUNTIME DESTINATION "release/${LIBRARY_NAME}/bin"
        PUBLIC_HEADER DESTINATION "release/${LIBRARY_NAME}")