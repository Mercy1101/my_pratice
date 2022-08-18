cmake_minimum_required(VERSION 3.5)

set(STATIC_LIB_NAME second_lib)

project(second)

add_library(${PROJECT_NAME} second.cpp)
add_library(${STATIC_LIB_NAME} ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/../../first
)
