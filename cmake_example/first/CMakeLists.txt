cmake_minimum_required(VERSION 3.5)

set(STATIC_LIB_NAME first_lib)

project(first)

add_library(${PROJECT_NAME} first.cpp)
add_library(${STATIC_LIB_NAME} ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}
)
