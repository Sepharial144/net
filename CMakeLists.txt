cmake_minimum_required (VERSION 3.23)

project(net VERSION 0.64 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(TARGET_OS STREQUAL "Win32")
    message("Target os: ${TARGET_OS}")

    find_library(WSOCK32_LIBRARY wsock32.lib)
    find_library(WS2_32_LIBRARY ws2_32.lib)
    find_library(ADVAPI32_LIBRARY AdvApi32.lib)

    message(STATUS "Library: ${WSOCK32_LIBRARY}" )

    set(LIB_DEPENDENCIES 
	    ${WSOCK32_LIBRARY} 
	    ${WS2_32_LIBRARY} 
	    ${ADVAPI32_LIBRARY}
    )
else()
endif()


file(GLOB_RECURSE HDR_FILES "source/*.hpp")
file(GLOB_RECURSE SRC_FILES "source/*.cpp")

set(SOURCE_DEPENDENCIES ${HDR_FILES} ${SRC_FILES})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(LIB_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

message("PATH: ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

add_library(${PROJECT_NAME} STATIC ${HDR_FILES} ${SRC_FILES})

set_target_properties(${PROJECT_NAME} 
    PROPERTIES 
    PUBLIC_HEADER "${HDR_FILES}")

target_link_libraries(${PROJECT_NAME} ${LIB_DEPENDENCIES})
target_include_directories(${PROJECT_NAME} PRIVATE "source")

#add_subdirectory("examples/client")
#add_subdirectory(tests)

# TODO: finish configure project
# library install in wrong folder
install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        ARCHIVE DESTINATION "debug/net/lib"
        PUBLIC_HEADER DESTINATION "debug/net/")
install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        LIBRARY DESTINATION "release/net"
        PUBLIC_HEADER DESTINATION "release/net")

#install(FILES ${IFC_FILES}
#       DESTINATION ${CMAKE_INSTALL_PREFIX}
#)