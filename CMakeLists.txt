cmake_minimum_required(VERSION 3.0)

project(wish)

add_executable(wish wish.c)

target_include_directories(wish
    PRIVATE
    ${PROJECT_SOURCE_DIR}
)