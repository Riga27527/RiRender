file(GLOB_RECURSE RIDER_CAMERAS_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(cameras STATIC ${RIDER_CAMERAS_SOURCES})
target_link_libraries(cameras PRIVATE core)
target_include_directories(cameras PUBLIC include)