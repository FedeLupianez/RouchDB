cmake_minimum_required(VERSION 3.10)
project(RouchDB C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c") 

add_executable(rouchdb ${SOURCES})

target_compile_options(rouchdb PRIVATE
    -O2
    -march=native
    -Wall
    -Wextra
)

find_package(OpenSSL REQUIRED)

target_include_directories(rouchdb PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(rouchdb PRIVATE OpenSSL::Crypto)
