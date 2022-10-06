CMAKE_MINIMUM_REQUIRED ( VERSION 3.15 )

set(CMAKE_CXX_STANDRAD 17)
set(CMAKE_CXX_STANDRAD_REQUIRED ON)

project(Rider LANGUAGES CXX C)
add_subdirectory(ext ext_build)

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
	src/shapes/*
	src/samplers/*
	src/textures/*
	src/lights/*
	)

include_directories(
	${PCG32_INCLUDE_DIR}
)

INCLUDE (CheckCXXSourceCompiles)
INCLUDE (CheckCXXSourceRuns)

CHECK_CXX_SOURCE_COMPILES (
"__attribute__((noinline)) void foo() { }
int main() { }"
HAVE_ATTRIBUTE_NOINLINE )

IF ( HAVE_ATTRIBUTE_NOINLINE )
  ADD_DEFINITIONS ( -D "RIDER_NOINLINE=__attribute__\\(\\(noinline\\)\\)" )
ELSEIF ( HAVE_DECLSPEC_NOINLINE )
  ADD_DEFINITIONS ( -D "RIDER_NOINLINE=__declspec(noinline)" )
ELSE ()
  ADD_DEFINITIONS ( -D RIDER_NOINLINE )
ENDIF ()

add_executable(Rider src/main.cpp ${source_files})
target_include_directories(Rider PUBLIC
	src/accelerators
	src/cameras
	src/core
	src/integrators
	src/materials
	src/shapes
	src/samplers
	src/textures
	src/lights
	)

find_package(TBB REQUIRED)
target_link_libraries(Rider PUBLIC TBB::tbb)