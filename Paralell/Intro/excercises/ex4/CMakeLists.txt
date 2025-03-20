cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Ex4)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(ex4 ex4.cpp)

# Link the pthread library
target_link_libraries(ex4 pthread)
#target_link_libraries(exc1LockGuards pthread)