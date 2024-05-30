cmake_minimum_required(VERSION 3.0.0)
project(cexim VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(main)
add_subdirectory(modules)

include(CTest)
enable_testing()