cmake_minimum_required(VERSION 3.3)
project(computer_graphics)

set(CMAKE_CXX_STANDARD 17)

set(PROGRAM_SOURCES
        src/main.cpp
        src/logger.cpp
        src/assets/shader.cpp
        src/assets/vertex_buffer.cpp
        src/assets/vertex_array.cpp
        src/assets/texture.cpp)

add_executable(computer_graphics ${PROGRAM_SOURCES})
target_compile_options(computer_graphics PUBLIC -Wall -Wextra)
target_include_directories(computer_graphics PUBLIC ${CMAKE_SOURCE_DIR}/include)

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/external/glad")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake/")
glad_add_library(glad STATIC API gl:core=4.6)
target_link_libraries(computer_graphics PUBLIC glad)

find_package(glfw3 REQUIRED)
target_link_libraries(computer_graphics PUBLIC glfw)

find_package(Boost REQUIRED COMPONENTS program_options)
target_link_libraries(computer_graphics PUBLIC Boost::program_options)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/spdlog")
spdlog_enable_warnings(computer_graphics)
target_link_libraries(computer_graphics PRIVATE spdlog::spdlog)
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/logs")
