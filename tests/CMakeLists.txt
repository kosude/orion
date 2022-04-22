# ---
# dependencies

set(DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/tests/deps")

set(ZML_USE_FLOATS ON CACHE BOOL "" FORCE)		# no docs
add_subdirectory("${DEPENDENCIES_DIR}/zetaml" "${DEPENDENCIES_DIR}/zetaml/build")

# ---
# glfwcompat: testing compatibility with third-party windowing APIs (in this case, GLFW)

add_executable(glfwcompat "glfwcompat.c")
target_link_libraries(glfwcompat ${PROJECT_NAME})
target_include_directories(glfwcompat PRIVATE "${PROJECT_SOURCE_DIR}/deps/glfw/include/GLFW")