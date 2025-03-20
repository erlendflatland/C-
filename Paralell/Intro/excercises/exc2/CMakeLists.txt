cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Exc2)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(ex2 ex2.cpp)

# Link the pthread library
target_link_libraries(ex2 pthread)
#target_link_libraries(exc1LockGuards pthread)