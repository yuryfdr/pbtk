project(sigc++)

file(GLOB_RECURSE SRC sigc++/*.cc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(sigc++ STATIC ${SRC})
