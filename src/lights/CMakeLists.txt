file(GLOB_RECURSE RIDER_LIGHTS_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(lights STATIC ${RIDER_LIGHTS_SOURCES})
target_link_libraries(lights PRIVATE core)
target_include_directories(lights PUBLIC include)