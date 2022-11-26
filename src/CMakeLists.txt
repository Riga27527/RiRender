
find_package(TBB REQUIRED)

add_library(rider-lib INTERFACE)

add_subdirectory(ext)
add_subdirectory(core)
add_subdirectory(accelerators)
add_subdirectory(cameras)
add_subdirectory(integrators)
add_subdirectory(lights)
add_subdirectory(materials)
add_subdirectory(samplers)
add_subdirectory(shapes)
add_subdirectory(textures)

target_link_libraries(rider-lib INTERFACE 
    rider_ext
    core
    TBB::tbb
    accelerators
    cameras
    integrators
    lights
    materials
    samplers
    shapes
    textures
)

add_subdirectory(app)