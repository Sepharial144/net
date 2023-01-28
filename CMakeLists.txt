cmake_minimum_required (VERSION 3.23)
#set(TARGET_NAME Net_server)
project(net VERSION 0.64)
set(TARGET_NAME Net_client)

find_library(WSOCK32_LIBRARY wsock32.lib)
find_library(WS2_32_LIBRARY ws2_32.lib)
find_library(ADVAPI32_LIBRARY AdvApi32.lib)

set(LIB_DEPENDENCIES 
	${WSOCK32_LIBRARY} 
	${WS2_32_LIBRARY} 
	${ADVAPI32_LIBRARY}
)

file(GLOB_RECURSE HDR_FILES "source/*.hpp")
file(GLOB_RECURSE SRC_FILES "source/*.cpp")

#add_executable(${TARGET_NAME} ${HDR_FILES} ${SRC_FILES})
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

#install(FILES logo.png
#        DESTINATION ${CMAKE_INSTALL_DOCDIR}/myproj
#)