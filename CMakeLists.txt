cmake_minimum_required(VERSION 2.18)
project(Noyan)

set(CMAKE_CXX_STANDARD 20)

find_package(SQLite3 REQUIRED)

#add_subdirectory(test)

include_directories(src)

set (SOURCES "src/Client.cpp"
	"src/Server.cpp"
	"src/SqliteDatabase.cpp"
    )

add_executable(Noyan "src/main.cpp" "${SOURCES}")

target_include_directories(Noyan PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_libraries(Noyan pthread ${SQLite3_LIBRARIES})
