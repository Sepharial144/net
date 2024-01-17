message(STATUS "Load ... config.cmake")

if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	set(DEBUG True)
    set(LOGGING True)
    add_compile_definitions(DEBUG=true)
else()
	set(DEBUG False)
    set(LOGGING False)
endif()

# Logging variables
# Set logging level, possible values
# Fatal            50000
# Error            40000
# Warn             30000
# Info             20000
# Debug            10000
# Debug - low       9000
# Debug - medium    8000
# Debug - hight     7000

if(LOGGING)
	set(LOGGING_FATAL_VALUE 50000)
	set(LOGGING_ERROR_VALUE 40000)
	set(LOGGING_WARNING_VALUE 30000)
	set(LOGGING_INFO_VALUE 20000)
	set(LOGGING_DEBUG_VALUE 10000)
	set(LOGGING_DEBUG_LOW_VALUE 9000)
	set(LOGGING_DEBUG_MEDIUM_VALUE 8000)
	set(LOGGING_DEBUG_HIGH_VALUE 7000)
endif()

set(LOG_LEVEL ${LOGGING_DEBUG_HIGH_VALUE})

add_compile_definitions(LOGGING=${LOGGING})
add_compile_definitions(LOGGING_LEVEL=${LOG_LEVEL})
add_compile_definitions(LOGGING_FATAL_VALUE=${LOGGING_FATAL_VALUE})
add_compile_definitions(LOGGING_ERROR_VALUE=${LOGGING_ERROR_VALUE})
add_compile_definitions(LOGGING_WARNING_VALUE=${LOGGING_WARNING_VALUE})
add_compile_definitions(LOGGING_INFO_VALUE=${LOGGING_INFO_VALUE})
add_compile_definitions(LOGGING_DEBUG_VALUE=${LOGGING_DEBUG_VALUE})
add_compile_definitions(LOGGING_DEBUG_LOW_VALUE=${LOGGING_DEBUG_LOW_VALUE})
add_compile_definitions(LOGGING_DEBUG_MEDIUM_VALUE=${LOGGING_DEBUG_MEDIUM_VALUE})
add_compile_definitions(LOGGING_DEBUG_HIGH_VALUE=${LOGGING_DEBUG_HIGH_VALUE})

message(STATUS "Net configuartion:")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "DEBUG: ${DEBUG}")
message(STATUS " logging: ${LOGGING}")
message(STATUS " log level: ${LOG_LEVEL}")