cmake_minimum_required(VERSION 3.10)
project(netsock C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)

set(SRC
    src/arguments.c
    src/logger.c
    src/network.c
    src/send.c
    src/recv.c
)

add_library(${PROJECT_NAME}_lib STATIC ${SRC})
target_include_directories(${PROJECT_NAME}_lib PUBLIC include)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)

find_package(GTest REQUIRED)
enable_testing()
file(GLOB TEST_SRCS "test/*.cpp")
add_executable(tests ${TEST_SRCS})
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main ${PROJECT_NAME}_lib)
add_test(NAME Tests COMMAND tests)
