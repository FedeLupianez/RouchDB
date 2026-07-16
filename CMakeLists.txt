cmake_minimum_required(VERSION 3.10)
project(RouchDB C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ALL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
list(FILTER ALL_SOURCES EXCLUDE REGEX "tcp_server\\.c$")
set(SOURCES ${ALL_SOURCES})

set(SERVER_SOURCES
    src/network/tcp_server.c
)

add_executable(rouchdb ${SOURCES})

add_executable(rouch_server ${SERVER_SOURCES})

target_compile_options(rouchdb PRIVATE
    -O2
    -march=native
    -Wall
    -Wextra
)

target_compile_options(rouch_server PRIVATE
    -O2
    -march=native
    -Wall
    -Wextra
)

find_package(OpenSSL REQUIRED)

target_include_directories(rouchdb PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_include_directories(rouch_server PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(rouchdb PRIVATE OpenSSL::Crypto)
