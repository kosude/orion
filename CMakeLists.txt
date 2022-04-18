cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

message("              _\n  ____  _____(_)___  ____\n / __ \\/ ___/ / __ \\/ __ \\\n/ /_/ / /  / / /_/ / / / /\n\\____/_/  /_/\\____/_/ /_/\n")

# ---
# project declaration

project(orion)

# ---
# interface options

option(BUILD_SHARED_LIBS "Build shared libraries." ON)
option(ORION_BUILD_LIB "Build the core Orion library." ON)
option(ORION_BUILD_TESTS "Build Orion test executable(s)." ON) # ON when testing!
option(ORION_BUILD_DOCS "Build Orion documentation." ON)

# ---
# subdirectories

if (ORION_BUILD_LIB)
	add_subdirectory("src")
endif()

if (ORION_BUILD_TESTS)
	add_subdirectory("tests")
endif()

if (ORION_BUILD_DOCS)
	#add_subdirectory("docs")
endif()