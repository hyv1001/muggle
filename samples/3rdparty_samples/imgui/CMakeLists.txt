cmake_minimum_required(VERSION 3.12)

project(imgui_example)

include(../../../cmake/common_marcos.cmake)

SETUP_SAMPLE(imgui_example "Samples/3rdParty")

target_link_libraries(imgui_example PUBLIC glad glfw glm imgui)