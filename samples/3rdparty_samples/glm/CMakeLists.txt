cmake_minimum_required(VERSION 3.12)

project(glm_example)

include(../../../cmake/common_marcos.cmake)

SETUP_SAMPLE(glm_example "Samples/3rdParty")

target_link_libraries(glm_example PUBLIC glad glfw glm muggle)