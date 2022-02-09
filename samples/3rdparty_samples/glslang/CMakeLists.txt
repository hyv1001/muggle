cmake_minimum_required(VERSION 3.12)

project(glslang_example)

include(../../../cmake/common_marcos.cmake)

SETUP_SAMPLE(glslang_example "Samples/3rdParty")

target_link_libraries(glslang_example PRIVATE glfw glslang SPIRV assimp)