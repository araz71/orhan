cmake_minimum_required(VERSION 2.18)
project(Noyan)

set(CMAKE_CXX_STANDARD 20) 

find_package(SQLite3 REQUIRED)
find_package(Protobuf REQUIRED)

#add_subdirectory(test)

include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(src)

set (SOURCES "src/Client.cpp"
	"src/Server.cpp"
	"src/SqliteDatabase.cpp"
	"src/RedisDatabase.cpp"
    )

protobuf_generate_cpp(${CMAKE_SOURCE_DIR} PROTO_HDRS "../configurations.proto")

add_executable(Noyan "src/main.cpp" "${SOURCES}")

target_include_directories(Noyan PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_libraries(Noyan pthread ${SQLite3_LIBRARIES})
target_link_libraries(Noyan ${PROTOBUF_LIBRARIES})
target_link_libraries(Noyan hiredis) 
