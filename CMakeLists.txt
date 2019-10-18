cmake_minimum_required(VERSION 3.10)
project(computer_graphics)

set(CMAKE_CXX_STANDARD 17)

set(PROGRAM_SOURCES
        src/main.cpp
        src/assets/shader.cpp)

add_executable(computer_graphics ${PROGRAM_SOURCES})

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/external/glad")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake/")
glad_add_library(glad STATIC API gl:core=4.6)
target_link_libraries(computer_graphics PUBLIC glad)

find_package(glfw3 REQUIRED)
target_link_libraries(computer_graphics PUBLIC glfw)

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(computer_graphics PUBLIC Boost::program_options)
