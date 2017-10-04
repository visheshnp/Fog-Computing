cmake_minimum_required(VERSION 2.8)
FILE(GLOB SRC *.cc)

SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

#set(CMAKE_CXX_STANDARD 11)

#include_directories(${CMAKE_BINARY_DIR}/proto)
#include_directories(${CMAKE_SOURCE_DIR}/src/common)
#include_directories(${CMAKE_SOURCE_DIR}/extlib/mongoose)
#add_library(src STATIC ${SRC})

add_subdirectory(proto)

find_package(PkgConfig)
pkg_check_modules(PC_ZeroMQ QUIET zmq)
find_path(ZeroMQ_INCLUDE_DIR NAMES zmq.hpp PATHS ${PC_ZeroMQ_INCLUDE_DIRS} )
find_library(ZeroMQ_LIBRARY NAMES zmq PATHS ${PC_ZeroMQ_LIBRARY_DIRS} )

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable(peerTopeer peerToPeerDealerRouter.cpp)
add_executable(centralRepo centralRepo.cpp)
target_include_directories(peerTopeer PUBLIC ${ZeroMQ_INCLUDE_DIR})
target_link_libraries(peerTopeer proto pthread ${ZeroMQ_LIBRARY} ${CURL_LIBRARIES} ${PROTOBUF_LIBRARY} ${Boost_LIBRARIES})
target_link_libraries(centralRepo proto pthread ${ZeroMQ_LIBRARY} ${CURL_LIBRARIES} ${PROTOBUF_LIBRARY} ${Boost_LIBRARIES})
