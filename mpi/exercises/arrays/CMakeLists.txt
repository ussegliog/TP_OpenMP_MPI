PROJECT(ARRAY_MPI_TP)

cmake_minimum_required(VERSION 2.6)

# Compilation options
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++14")

# includes and sources
INCLUDE_DIRECTORIES(${ARRAY_MPI_TP_SOURCE_DIR}/array_lib)
FILE(GLOB array_CXX "${ARRAY_MPI_TP_SOURCE_DIR}/array_lib/*.cpp")

# rk4Lib library
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/array_lib)
ADD_LIBRARY(arrayLib SHARED ${array_CXX})

# Binaries
ADD_EXECUTABLE(sort_single_array_exe sort_single_array_program.cpp)
TARGET_LINK_LIBRARIES(sort_single_array_exe arrayLib)

ADD_EXECUTABLE(sort_several_arrays_exe sort_several_arrays_program.cpp)
TARGET_LINK_LIBRARIES(sort_several_arrays_exe arrayLib)
