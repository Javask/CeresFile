cmake_minimum_required(VERSION 3.7)

project(CeresFile)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DCERESFILE_DEBUG)
endif()

if(UNIX)
    add_definitions(-DCERESFILE_PLATFORM_LINUX)
elseif(WIN32)
    add_definitions(-DCERESFILE_PLATFORM_WINDOWS)
endif()

file(GLOB_RECURSE CERESFILE_HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.h"
							   "${CMAKE_CURRENT_LIST_DIR}/src/*.hpp")
file(GLOB_RECURSE CERESFILE_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

set(CeresFile_INDLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/include")
file(GLOB_RECURSE CERESFILE_INCLUDE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")


add_library(CeresFile STATIC ${CERESFILE_SOURCE_FILES} ${CERESFILE_HEADER_FILES})
target_include_directories(CeresFile INTERFACE "${CMAKE_CURRENT_LIST_DIR}/include")
add_library(CeresFile::CeresFile ALIAS CeresFile)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(CERESFILE_BUILD_TESTS_DEFAULT ON)
else()
  message(STATUS "CeresFile running as subproject!")
  set(CERESFILE_BUILD_TESTS_DEFAULT OFF)
endif()
option(CERESFILE_BUILD_TESTS "Build CeresFile Testing Project" ${CERESFILE_BUILD_TESTS_DEFAULT})
if(CERESFILE_BUILD_TESTS)
  message(STATUS "Building CeresFile tests...")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/thirdparty/catch")
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test/")
else()
  message(STATUS "CeresFile-Tests disabled..")
endif()

