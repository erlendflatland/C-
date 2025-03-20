# C-
Forskjellige c++ oppgaver etc

## G++ compilation
g++ exc1.cpp -o exc1.exe
###
./exc1.exe

## CMAKE
cmake_minimum_required(VERSION 3.10)
### CMAKEList
#### Set the project name
project(Exc1)

#### Add the executable
add_executable(exc1 exc1.cpp)

#### Link the pthread library
target_link_libraries(exc1 pthread)

### Building
mkdir build
cd build
cmake ..
make


### Run
./exc1
