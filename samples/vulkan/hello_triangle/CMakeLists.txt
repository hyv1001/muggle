cmake_minimum_required(VERSION 3.12)

project(hello_triangle)

include(../../../cmake/common_marcos.cmake)

SETUP_SAMPLE(hello_triangle "Samples/Vulkan")

target_link_libraries(hello_triangle PUBLIC glad glfw muggle)