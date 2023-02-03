cmake_minimum_required(VERSION .218)

project(Noyan)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

set (SOURCES src/Client.cpp)

add_executable(Noyan src/main.cpp ${SOURCES})

target_link_libraries(Noyan pthread)
