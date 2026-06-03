cmake_minimum_required(VERSION 3.10)
project(RouchDB C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c") 

add_executable(rouchdb ${SOURCES})

target_include_directories(rouchdb PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
