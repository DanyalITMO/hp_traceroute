cmake_minimum_required(VERSION 3.0)
project(sender)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
set(CMAKE_CXX_STANDARD 17)

add_executable(sender main.cpp stats.cpp stats.h utils.cpp utils.h arp.cpp arp.h fillers.cpp fillers.h route.cpp config.cpp handler_ipv4.cpp)
