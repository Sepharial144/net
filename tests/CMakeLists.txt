find_package(GTest REQUIRED)
enable_testing()

set(TARGET net-unit)

file(GLOB_RECURSE TEST_SRC_FILES "unit/*.hpp" "unit/*.cpp")

add_executable(${TARGET} ${TEST_SRC_FILES})

target_link_libraries(${TARGET} GTest::GTest GTest::Main)

target_include_directories(${TARGET} PUBLIC "${CMAKE_SOURCE_DIR}/source")