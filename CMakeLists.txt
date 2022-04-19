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
option(ORION_BUILD_DOCS "Build Orion documentation." ON)

# ---
# configure files

configure_file("${PROJECT_SOURCE_DIR}/CMake/generate/platform.h.in" "${PROJECT_BINARY_DIR}/generated/platform.h" @ONLY)

# ---
# subdirectories

if (ORION_BUILD_LIB)
	add_subdirectory("src")
endif()

if (ORION_BUILD_TESTS)
	add_subdirectory("tests")
endif()

if (ORION_BUILD_DOCS)
	add_subdirectory("docs") 
endif()