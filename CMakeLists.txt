cmake_minimum_required(VERSION 3.18)

project(CeresFile VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_ASAN "-fsanitize=address -fno-omit-frame-pointer -g -Wall")
set(CMAKE_EXE_LINKER_FLAGS_ASAN  "-fsanitize=address -g -Wall")

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

if(UNIX)
    add_definitions(-DCERESFILE_PLATFORM_LINUX)
elseif(WIN32)
    add_definitions(-DCERESFILE_PLATFORM_WINDOWS)
endif()

file(GLOB_RECURSE CERESFILE_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
add_library(CeresFile STATIC ${CERESFILE_SOURCE_FILES})
set_target_properties(CeresFile PROPERTIES CXX_STANDARD 20)

target_include_directories(CeresFile
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(CeresFile::CeresFile ALIAS CeresFile)

option(CERESFILE_BUILD_TESTS "Build CeresFile Tests" ON)
if(CERESFILE_BUILD_TESTS)
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test/")
endif()

