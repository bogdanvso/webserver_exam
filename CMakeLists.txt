cmake_minimum_required(VERSION 2.8) 						
project(webserver)		

FIND_PACKAGE(Threads REQUIRED)

#includes
include_directories("include/" "asio-1.12.1/include")

#sources
file(GLOB SOURCES "src/*.cpp")

#defintions
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DASIO_STANDALONE") #-Wall

add_executable(final ${SOURCES})

#link libs 
target_link_libraries( final pthread )									