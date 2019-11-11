cmake_minimum_required(VERSION 3.3)
project(computer_graphics)

set(CMAKE_CXX_STANDARD 17)

set(PROGRAM_SOURCES
        src/main.cpp
        src/logger.cpp
        src/assets/shader.cpp
        src/assets/vertex_buffer.cpp
        src/assets/vertex_array.cpp
        src/assets/texture.cpp
        src/argument.cpp)

set(EXTERNAL_DIRECTORY ${PROJECT_SOURCE_DIR}/external)

add_executable(computer_graphics ${PROGRAM_SOURCES})
if(MSVC)
    target_compile_options(computer_graphics INTERFACE /W4)
else()
    target_compile_options(computer_graphics INTERFACE -Wall -Wextra)
endif()
target_include_directories(computer_graphics PUBLIC ${CMAKE_SOURCE_DIR}/include)

set(GLAD_SOURCES_DIR "${EXTERNAL_DIRECTORY}/glad")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake/")
glad_add_library(glad STATIC API gl:core=4.6)
target_link_libraries(computer_graphics PUBLIC glad)

add_subdirectory("${EXTERNAL_DIRECTORY}/glfw")
target_include_directories(computer_graphics PUBLIC ${EXTERNAL_DIRECTORY}/glfw/include)
target_link_libraries(computer_graphics PUBLIC glfw)

add_subdirectory("${EXTERNAL_DIRECTORY}/cxxopts")
target_link_libraries(computer_graphics PUBLIC cxxopts)

add_subdirectory("${EXTERNAL_DIRECTORY}/spdlog")
spdlog_enable_warnings(computer_graphics)
target_link_libraries(computer_graphics PUBLIC spdlog::spdlog)
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/logs")
