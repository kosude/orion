# ---
# build GLFW

set(DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/tests/deps")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)		# no docs
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)		# no tests
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)	# no examples

add_subdirectory("${DEPENDENCIES_DIR}/glfw" "${DEPENDENCIES_DIR}/glfw/build")

# ---
# interface: testing the general Orion public interface

add_executable(interface "interface.c")
target_link_libraries(interface ${PROJECT_NAME} glfw)
target_include_directories(interface PRIVATE "${DEPENDENCIES_DIR}/glfw/include/GLFW")