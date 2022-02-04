include(CTest)
include("${CMAKE_CURRENT_LIST_DIR}/../thirdparty/catch/contrib/Catch.cmake")
enable_testing()

file(GLOB_RECURSE CERESFILE_TEST_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.h"
							   "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
set(CERESFILE_TEST_PROJECT_NAME "CeresFile_Test")

add_executable(CeresFile_Test ${CERESFILE_TEST_FILES})
target_link_libraries(CeresFile_Test Catch2::Catch2 CeresFile)
add_dependencies(CeresFile_Test Catch2::Catch2 CeresFile)

catch_discover_tests(CeresFile_Test)