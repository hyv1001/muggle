cmake_minimum_required(VERSION 3.12)

project(glfw_example)

include(../../../cmake/common_marcos.cmake)

SETUP_SAMPLE(glfw_example "Samples/3rdParty")

target_link_libraries(glfw_example PUBLIC glad glfw)