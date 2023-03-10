cmake_minimum_required(VERSION 2.18)
project(Noyan)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED)

include_directories(src)

set (SOURCES "src/Client.cpp"
	"src/Server.cpp"
    )

add_executable(Noyan "src/main.cpp" "${SOURCES}")

target_link_libraries(Noyan pthread boost sqlite3)
