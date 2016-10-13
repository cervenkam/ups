cmake_minimum_required(VERSION 2.8)
project(Cards)

set(CMAKE_CXX_FLAGS "-Wl,--no-as-needed -std=c++11 -Wall -Wextra -Wno-conversion-null -g -lpthread")
set(SRCS
	commands.cxx
	commonnetwork.cxx
	server.cxx
	client.cxx
)

add_library(networking STATIC ${SRCS})
#target_compile_definitions(networking PRIVATE LANG_CZ)
