file(GLOB_RECURSE RIDER_TEXTURES_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(textures STATIC ${RIDER_TEXTURES_SOURCES})
target_link_libraries(textures PRIVATE core)
target_include_directories(textures PUBLIC include)