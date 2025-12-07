cmake_minimum_required(VERSION 4.0)

project(image)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE INCLUDES ${CMAKE_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDES})
