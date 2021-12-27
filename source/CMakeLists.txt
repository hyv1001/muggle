
file(GLOB muggle_src
    foundation/log/*.h
    foundation/log/*.cpp)

add_library(muggle STATIC EXCLUDE_FROM_ALL ${muggle_src})
target_include_directories(muggle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
