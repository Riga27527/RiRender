file(GLOB_RECURSE RIDER_MATERIALS_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(materials STATIC ${RIDER_MATERIALS_SOURCES})
target_link_libraries(materials PRIVATE core)
target_include_directories(materials PUBLIC include)