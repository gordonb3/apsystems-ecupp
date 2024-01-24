cmake_minimum_required(VERSION 3.16.0)

project(APSystems_ECU)

option(ENABLE_DEBUG "Enable debugging" NO)


if(ENABLE_DEBUG)
    add_definitions(-DDEBUG)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CXX_EXTENSIONS NO)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../.lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../.lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/..)


# main include dirs
include_directories(${CMAKE_SOURCE_DIR}/../src)


## Sources

# Targets
file(GLOB ECU_demo_SRCS *.cpp)


# Main library
file(GLOB_RECURSE ECU_client_SRCS ../src/*.cpp)
add_library(APSystems_ECU STATIC ${ECU_client_SRCS})


foreach(demo ${ECU_demo_SRCS})
  get_filename_component(exefile ${demo} NAME_WE)
  add_executable(${exefile} ${demo})
  target_link_libraries(${exefile} APSystems_ECU ${ZLIB_LIBRARIES} pthread ${OPENSSL_LIBRARIES})
  message(STATUS "Created make target ${exefile}")
endforeach(demo)

