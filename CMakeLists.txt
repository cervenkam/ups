cmake_minimum_required(VERSION 2.8)
project(Cards)

set(CMAKE_CXX_FLAGS "-Wl,--no-as-needed -std=c++11 -Wall -Wextra -Wno-conversion-null -g -lpthread")
set(SRCS
	semaphore.cxx
	config.cxx
	programmerbot.cxx
	networkplayer.cxx
	person.cxx
	algono.cxx
	algorithm.cxx
	game.cxx
	hand.cxx
	deck.cxx
	card.cxx
	main.cxx
)

add_subdirectory(networking)
add_executable(server ${SRCS})
#add_executable(client ${SRCS})
target_link_libraries(server networking)
#target_link_libraries(client networking)
target_compile_definitions(server PRIVATE SERVER)
#target_compile_definitions(server PRIVATE LANG_CZ SERVER)
#target_compile_definitions(client PRIVATE LANG_CZ)
