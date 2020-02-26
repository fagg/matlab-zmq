cmake_minimum_required(VERSION 3.15)
project(zmq_publisher)

set(CMAKE_CXX_STANDARD 11)


set(SOURCE_FILES main.cpp src/datapoints.cpp)
set(HEADER_FILES include/datapoints.h)

add_executable(zmq_publisher ${SOURCE_FILES} ${HEADER_FILES})


MESSAGE(STATUS "ZeroMQ_FOUND: " ${ZeroMQ_FOUND})
MESSAGE(STATUS "ZeroMQ_LIBRARIES: " ${ZeroMQ_LIBRARIES})
MESSAGE(STATUS "ZeroMQ_INCLUDE_DIR: " ${ZeroMQ_INCLUDE_DIR})
MESSAGE(STATUS "ZeroMQ_VERSION: " ${ZeroMQ_VERSION})


## zmq
## Armadillo Libraries
include_directories(include ${ZeroMQ_INCLUDE_DIRS} /usr/local/include)

find_library(ZeroMQ_LIBRARIES armadillo /usr/local/lib)
target_link_libraries(zmq_publisher  ${ZeroMQ_LIBRARIES} /usr/local/lib)





##find_package(ZeroMQ 4.3.3 REQUIRED)

## Armadillo Libraries
find_package(Armadillo 4.4 REQUIRED)
include_directories(include ${ARMADILLO_INCLUDE_DIRS})

find_library(ARMADILLO_LIBRARIES armadillo)
target_link_libraries(zmq_publisher  ${ARMADILLO_LIBRARIES})


