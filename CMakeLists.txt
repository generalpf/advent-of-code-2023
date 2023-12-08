cmake_minimum_required(VERSION 3.27)

project(
  AdventOfCode2023
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB source_files "src/*.cpp")
foreach(filename ${source_files})
    string(REPLACE ".cpp" "" filename_no_extension ${filename})
    string(REPLACE "${CMAKE_SOURCE_DIR}/src/" "" filename_no_path ${filename_no_extension})
    add_executable(${filename_no_path} ${filename})
endforeach()
