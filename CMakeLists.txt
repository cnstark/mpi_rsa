cmake_minimum_required(VERSION 3.15)
project(mpi_rsa)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(mpi_rsa
        src/test.cpp
        src/rsa.cpp
        src/rsa.hpp)
