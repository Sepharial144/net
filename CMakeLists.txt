cmake_minimum_required (VERSION 3.23)

project(net VERSION 0.64 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    find_library(WSOCK32_LIBRARY wsock32.lib)
    find_library(WS2_32_LIBRARY ws2_32.lib)
    find_library(ADVAPI32_LIBRARY AdvApi32.lib)

    set(IFC_FILES
        "source/net.hpp"
        "source/socket_definition.hpp"
        "source/win32/win32_definitions.hpp"
        "source/win32/net_server.hpp"
        "source/win32/net_client.hpp"
        "source/win32/net_connection.hpp"
    )

    set(LIB_DEPENDENCIES 
	    ${WSOCK32_LIBRARY} 
	    ${WS2_32_LIBRARY} 
	    ${ADVAPI32_LIBRARY}
    )
elseif(Linux)
endif()

file(GLOB_RECURSE HDR_FILES "source/*.hpp")
file(GLOB_RECURSE SRC_FILES "source/*.cpp")

add_library(${PROJECT_NAME} STATIC ${HDR_FILES} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${LIB_DEPENDENCIES})
target_include_directories(${PROJECT_NAME} PRIVATE "source")

#add_subdirectory(examples)
#add_subdirectory(tests)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Debug
        LIBRARY DESTINATION "include/release/net")
install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS Release
        LIBRARY DESTINATION  "include/debug/net")

install(FILES ${IFC_FILES}
       DESTINATION "include/debug/net"
)