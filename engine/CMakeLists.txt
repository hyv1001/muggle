
file(GLOB_RECURSE muggle_src *.h *.cpp)

add_library(muggle STATIC EXCLUDE_FROM_ALL ${muggle_src})
target_include_directories(muggle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(muggle spdlog nlohmann_json Vulkan::Vulkan)

target_link_libraries(muggle
    debug ${Vulkan_shaderc_combined_LIBRARY}/../shaderc_combinedd.lib
    optimized Vulkan::shaderc_combined)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${muggle_src})

set_target_properties(muggle PROPERTIES FOLDER "Engine")