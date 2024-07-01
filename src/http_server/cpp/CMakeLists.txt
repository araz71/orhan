cmake_minimum_required(VERSION 3.2)
project(HTTP_SERVER)

# Find the MySQL Connector/C++ package
find_package(mysql-concpp REQUIRED)

add_executable(Main Main.cpp)

# Link the MySQL Connector/C++ library to the target
target_link_libraries(myapp mysql::concpp)

