file(GLOB_RECURSE RIDER_INTEGRATOR_SOURCES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(integrators STATIC ${RIDER_INTEGRATOR_SOURCES})
target_link_libraries(integrators PRIVATE core)
target_include_directories(integrators PUBLIC include)