# set(CMAKE_CXX_STANDRAD 17)
# set(CMAKE_CXX_STANDRAD_REQUIRED ON)

# if (NOT CMAKE_BUILD_TYPE)
# 	set(CMAKE_BUILD_TYPE Release)
# endif()

# set(PCG32_INCLUDE_DIR
#   ${CMAKE_CURRENT_SOURCE_DIR}/pcg32)

# set(CompilerFlags PCG32_INCLUDE_DIR)

# foreach(CompilerFlag ${CompilerFlags})
#   set(${CompilerFlag} "${${CompilerFlag}}" PARENT_SCOPE)
# endforeach()

add_library(rider_ext INTERFACE)

add_library(pcg32 INTERFACE)
target_include_directories(pcg32 INTERFACE pcg32/include)
target_link_libraries(rider_ext INTERFACE pcg32)