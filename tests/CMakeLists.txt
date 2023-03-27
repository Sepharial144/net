#find_package(GTest REQUIRED)
find_package(GTest CONFIG REQUIRED)
enable_testing()
message(STATUS "Test framework: ${GTest}")


file(GLOB_RECURSE UNIT_SRC_FILES "unit/*.hpp" "unit/*.cpp")
file(GLOB_RECURSE API_SRC_FILES "api/*.hpp" "api/*.cpp")

function(add_custom_test nameTest srcFiles)
    set(EXECUTION_TEST_FILE ${PROJECT_NAME}-${nameTest}-test)
	set(NAME_TEST ${PROJECT_NAME}-${nameTest}-gtest)
	add_executable(${EXECUTION_TEST_FILE} ${srcFiles} ${SOURCE_DEPENDENCIES})
	target_link_libraries(${EXECUTION_TEST_FILE} PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
	#target_link_libraries(${EXECUTION_TEST_FILE} PUBLIC GTest::GTest GTest::Main)
	target_include_directories(${EXECUTION_TEST_FILE} PUBLIC ${NET_INCLUDE_DIR})
	add_test(${NAME_TEST} ${EXECUTION_TEST_FILE})
endfunction()

add_custom_test(unit ${UNIT_SRC_FILES})
add_custom_test(api  ${API_SRC_FILES})

