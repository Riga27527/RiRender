CMAKE_MINIMUM_REQUIRED ( VERSION 3.15 )

set(CMAKE_CXX_STANDRAD 17)
set(CMAKE_CXX_STANDRAD_REQUIRED ON)

project(Rider LANGUAGES CXX C)

if (PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
	message(WARNING "The binary directory of CMake cannot be the same as source directory!")
endif()

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

if(WIN32)
	add_definitions(-DNOMINMAX -D_USE_MATH_DEFINES)
endif()

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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_subdirectory(src)