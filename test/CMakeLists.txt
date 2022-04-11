cmake_minimum_required(VERSION 3.18)

project(CeresFileTests)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/CPM.cmake)

#add Catch2
CPMAddPackage("gh:catchorg/Catch2@2.13.8")

file(GLOB_RECURSE CERESFILE_TEST_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.h"
							   "${CMAKE_CURRENT_LIST_DIR}/*.cpp")


add_executable(CeresFileTests ${CERESFILE_TEST_FILES})
target_link_libraries(CeresFileTests $<TARGET_NAME_IF_EXISTS:Catch2::Catch2> CeresFile::CeresFile)
add_dependencies(CeresFileTests CeresFile::CeresFile)
set_target_properties(CeresFileTests PROPERTIES CXX_STANDARD 20 COMPILE_OPTIONS "${CERES_CXX_FLAGS}")
target_include_directories(CeresFileTests PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../src" "${CMAKE_CURRENT_LIST_DIR}/../include")


enable_testing()
add_test(CeresFileTests CeresFileTests)