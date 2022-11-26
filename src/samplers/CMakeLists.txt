file(GLOB_RECURSE RIDER_SAMPLERS_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(samplers STATIC ${RIDER_SAMPLERS_SOURCES})
target_link_libraries(samplers PRIVATE core)
target_include_directories(samplers PUBLIC include)