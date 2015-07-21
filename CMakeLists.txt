# needed for cmake build system
cmake_minimum_required (VERSION 2.6)

# the name of the project
project (gpsproto)

# c++ flags 
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-D DEBUG")

# protobuf library 
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/proto)

add_subdirectory(${CMAKE_SOURCE_DIR}/proto)

# add executable
set(EXECUTABLE gpsproto)
set(EXECUTABLE_SRC gpsproto.cpp)
add_executable(${EXECUTABLE} ${EXECUTABLE_SRC})
target_link_libraries(${EXECUTABLE} proto ${PROTOBUF_LIBRARIES})





 
   
