cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---
# ascii message for fun

message("              _\n  ____  _____(_)___  ____\n / __ \\/ ___/ / __ \\/ __ \\\n/ /_/ / /  / / /_/ / / / /\n\\____/_/  /_/\\____/_/ /_/\n")

# ---
# project declaration

project(orion)

set(DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/deps")

# ---
# interface options

option(BUILD_SHARED_LIBS "Build shared libraries." ON)
option(ORION_BUILD_LIB "Build the core Orion library." ON)
option(ORION_BUILD_TESTS "Build Orion test executable(s)." OFF)
option(ORION_BUILD_EXAMPLES "Build Orion usage example executable(s)." OFF)
option(ORION_BUILD_DOCS "Build Orion documentation." ON)

# ---
# configure files

configure_file("${PROJECT_SOURCE_DIR}/CMake/generate/platform.h.in" "${PROJECT_BINARY_DIR}/generated/platform.h" @ONLY)

# ---
# executables' dependencies are included if necessary
if (ORION_BUILD_TESTS OR ORION_BUILD_EXAMPLES)
    # zetaml
    set(ZML_USE_FLOATS ON CACHE BOOL "" FORCE)
    add_subdirectory("${DEPENDENCIES_DIR}/execdeps/zetaml" "${DEPENDENCIES_DIR}/execdeps/zetaml/build")

    # glm
    add_subdirectory("${DEPENDENCIES_DIR}/execdeps/glm" "${DEPENDENCIES_DIR}/execdeps/glm/build")
endif()

# ---
# subdirectories

if (ORION_BUILD_LIB)
    message(STATUS "ORION :: Compiling core Orion library")
    add_subdirectory("src")
endif()

if (ORION_BUILD_TESTS)
    message(STATUS "ORION :: Compiling Orion tests")
    add_subdirectory("tests")
endif()

if (ORION_BUILD_EXAMPLES)
    message(STATUS "ORION :: Compiling Orion examples")
    add_subdirectory("examples")
endif()

if (ORION_BUILD_DOCS)
    message(STATUS "ORION :: Compiling Orion HTML documentation")
    add_subdirectory("docs") 
endif()

# ---
# treat warnings as errors

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()
