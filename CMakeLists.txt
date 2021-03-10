cmake_minimum_required(VERSION 3.5)

project(inihandle LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB SRC_FILES "*.cpp" "*.cc" "*.h")

add_executable(inihandle ${SRC_FILES})
