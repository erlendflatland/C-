cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Exc1)

# Add the executable
add_executable(exc1 exc1.cpp)

add_executable(exc1LockGuards exc1LockGuards.cpp)


# Link the pthread library
target_link_libraries(exc1 pthread)
target_link_libraries(exc1LockGuards pthread)