file(GLOB_RECURSE RIDER_SHAPES_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(shapes STATIC ${RIDER_SHAPES_SOURCES})
target_link_libraries(shapes PRIVATE core)
target_include_directories(shapes PUBLIC include)