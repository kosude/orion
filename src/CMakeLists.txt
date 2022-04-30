# ---
# add source files to library output

set(SRC
    "buffers.c"
    "callback.c"
    "init.c"
    "internal.h"
    "shaders.c"
    "textures.c"
    "window.c"
)

if (BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${SRC})
else()
    add_library(${PROJECT_NAME} STATIC ${SRC})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}/generated")
target_include_directories(${PROJECT_NAME} PUBLIC "${DEPENDENCIES_DIR}")

# ---
# dependencies

set(build_shared_libs_cache "${BUILD_SHARED_LIBS}")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)        # no docs
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)       # no tests
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)    # no examples
# (build and link):
add_subdirectory("${DEPENDENCIES_DIR}/glfw" "${DEPENDENCIES_DIR}/glfw/build")
target_link_libraries(${PROJECT_NAME} glfw)

# other (non-CMake) dependencies
target_sources(${PROJECT_NAME} PRIVATE
    "${DEPENDENCIES_DIR}/glad/4.6/glad.c"
    "${DEPENDENCIES_DIR}/glad/4.6/orionglad/orionglad.c"
)

set(BUILD_SHARED_LIBS ${build_shared_libs_cache} CACHE BOOL "" FORCE)

# ---
# define macro based on configuration type via platform.h file.

if (CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug Release)
elseif (NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "Missing build configuration type! Reconfigure project with option -DCMAKE_BUILD_TYPE=(Debug|Release)")
endif()
set_property(GLOBAL PROPERTY DEBUG_CONFIGURATIONS Debug)

if (CMAKE_BUILD_TYPE)
    string(TOUPPER "${CMAKE_BUILD_TYPE}" _upperBuildType)
    set("BUILD_${_upperBuildType}" 1 CACHE INTERNAL "")
endif()

# ---
# status messages

if (WIN32)
    message(STATUS " => Platform:\t\tWindows")
elseif (APPLE)
    message(STATUS " => Platform:\t\tmacOS")
elseif (UNIX)
    message(STATUS " => Platform:\t\tUnix")
endif()

message(STATUS " => Compiler:\t\t${CMAKE_C_COMPILER_ID} (at ${CMAKE_C_COMPILER})")
message(STATUS " => Compiler version:\t${CMAKE_C_COMPILER_VERSION}")

message(STATUS " => Configuration:\t\t${_upperBuildType}")

# ---
# add shader presets

add_subdirectory(presets)
