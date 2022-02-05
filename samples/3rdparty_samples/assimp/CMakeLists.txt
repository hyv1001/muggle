cmake_minimum_required(VERSION 3.12)

project(assimp_example)

include(../../../cmake/common_marcos.cmake)

SETUP_SAMPLE(assimp_example "Samples/3rdParty")

target_link_libraries(assimp_example PUBLIC glad glfw glm assimp muggle)