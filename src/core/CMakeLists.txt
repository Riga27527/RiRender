file(GLOB_RECURSE RIDER_CORE_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(core STATIC ${RIDER_CORE_SOURCES})
target_link_libraries(core PUBLIC rider_ext)
target_include_directories(core PUBLIC include)