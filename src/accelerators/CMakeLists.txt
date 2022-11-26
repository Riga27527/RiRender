file(GLOB_RECURSE RIDER_ACCELERATORS_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(accelerators STATIC ${RIDER_ACCELERATORS_SOURCES})
target_link_libraries(accelerators PRIVATE core)
target_include_directories(accelerators PUBLIC include)