cmake_minimum_required(VERSION 3.18)

project(CeresFile VERSION 1.0.0 LANGUAGES CXX)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/CPM.cmake)

option(CERESFILE_BUILD_OWN_PACKAGES "Build all dependencies from scratch." OFF)
if(NOT CERESFILE_BUILD_OWN_PACKAGES)
  set(CPM_USE_LOCAL_PACKAGES ON)
else()
  message(STATUS "Building CeresFile dependencies from scratch...")
  set(CPM_USE_LOCAL_PACKAGES OFF)
endif()

set(CMAKE_CXX_FLAGS_ASAN "-fsanitize=address -fno-omit-frame-pointer -g -Wall")
set(CMAKE_EXE_LINKER_FLAGS_ASAN  "-fsanitize=address -g -Wall")

if(MSVC)
  set(CERES_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
else()
  set(CERES_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic)
endif()

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

if(UNIX)
    add_definitions(-DCERESFILE_PLATFORM_LINUX)
elseif(WIN32)
    add_definitions(-DCERESFILE_PLATFORM_WINDOWS)
endif()


CPMAddPackage(NAME Utilities 
              GIT_TAG v1.0.0-alpha5
              GITHUB_REPOSITORY Javask/Utilities
              OPTIONS
                "UTILITIES_BUILD_TESTS OFF")

file(GLOB_RECURSE CERESFILE_SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
add_library(CeresFile STATIC ${CERESFILE_SOURCE_FILES})
set_target_properties(CeresFile PROPERTIES CXX_STANDARD 20 COMPILE_OPTIONS "${CERES_CXX_FLAGS}")
target_link_libraries(CeresFile Utilities::Utilities)
if(UNIX)
    target_link_libraries(CeresFile pthread)
endif()

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
  enable_testing()
  add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test/")
endif()

