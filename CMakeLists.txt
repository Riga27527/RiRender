CMAKE_MINIMUM_REQUIRED ( VERSION 3.15 )

set(CMAKE_CXX_STANDRAD 17)
set(CMAKE_CXX_STANDRAD_REQUIRED ON)

project(Rider LANGUAGES CXX C)
# add_subdirectory(ext ext_build)

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
	message(WARNING "The binary directory of CMake cannot be the same as source directory!")
endif()

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

if(WIN32)
	add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

file(GLOB source_files CONFIGURE_DEPENDS 
	src/accelerators/*
	src/cameras/*
	src/core/*
	src/integrators/*
	src/materials/*
	src/shapes/*)

# add_library(Rider_lib STATIC ${source_files})
add_executable(Rider src/main.cpp)
target_sources(Rider PUBLIC ${source_files})
# target_link_libraries(Rider PUBLIC Rider_lib)

find_package(TBB REQUIRED)
target_link_libraries(Rider PUBLIC TBB::tbb)