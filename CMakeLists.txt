cmake_minimum_required(VERSION 3.0)

project(wish)

add_executable(wish src/wish.c src/parser.c)

target_include_directories(wish
    PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include
)

set(CMAKE_BUILD_TYPE "Debug")