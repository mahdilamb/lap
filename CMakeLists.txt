cmake_minimum_required(VERSION 3.0.0)
project(lapjv VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()
include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src)
add_library(liblapjv src/lapjv.cpp)
add_executable(lapjv main.cpp src/lapjv.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
