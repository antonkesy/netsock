cmake_minimum_required(VERSION 3.10)
project(netsock C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall")

set(SRC
    src/netsock.c
    src/arguments/arguments.c
    src/network/network.c
    src/network/send/send.c
    src/network/recv/recv.c
)

add_executable(netsock ${SRC})

target_include_directories(netsock PRIVATE src)
