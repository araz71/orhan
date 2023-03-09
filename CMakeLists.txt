cmake_minimum_required(VERSION 2.18)
project(Noyan)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED)

set(OSFSM_DIR "~/osfsm/osfsm")

include_directories(src)
include_directories("${OSFSM_DIR/comm_protocol/src}")

set (SOURCES "src/Client.cpp"
	"src/Server.cpp"
	"${OSFSM_DIR}/comm_protocol/src/danish.c"
    )

add_executable(Noyan "src/main.cpp" "${SOURCES}")

target_link_libraries(Noyan pthread boost)
