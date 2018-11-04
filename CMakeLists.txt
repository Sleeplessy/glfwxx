cmake_minimum_required(VERSION 3.12)
project(glfwxx)

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src GLFWXX_SRC)
add_library(glfwxx ${GLFWXX_SRC})
target_link_libraries(glfwxx pthread)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/unit_test)