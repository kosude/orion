# ---
# glfwcompat: demonstrating compatibility with third-party windowing APIs (in this case, GLFW)

add_executable(glfwcompat "glfwcompat.c")
target_link_libraries(glfwcompat ${PROJECT_NAME} zetaml)
target_include_directories(glfwcompat PRIVATE "${PROJECT_SOURCE_DIR}/deps/glfw/include/GLFW")