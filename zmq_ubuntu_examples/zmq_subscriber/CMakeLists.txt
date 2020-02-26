cmake_minimum_required(VERSION 3.15)
project(zmq_subscriber)

set(CMAKE_CXX_STANDARD 11)


add_executable(zmq_subscriber main.cpp)


MESSAGE(STATUS "ZeroMQ_FOUND: " ${ZeroMQ_FOUND})
MESSAGE(STATUS "ZeroMQ_LIBRARIES: " ${ZeroMQ_LIBRARIES})
MESSAGE(STATUS "ZeroMQ_INCLUDE_DIR: " ${ZeroMQ_INCLUDE_DIR})
MESSAGE(STATUS "ZeroMQ_VERSION: " ${ZeroMQ_VERSION})


##
## zmq Libraries
include_directories(include ${ZeroMQ_INCLUDE_DIRS} /usr/local/include)

find_library(ZeroMQ_LIBRARIES /usr/local/lib)
target_link_libraries(zmq_subscriber  ${ZeroMQ_LIBRARIES} /usr/local/lib)



