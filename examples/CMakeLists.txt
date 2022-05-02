# ---
# glfwcompat: demonstrating compatibility with third-party windowing APIs (in this case, GLFW)

add_executable(glfwcompat "glfwcompat.c")
target_link_libraries(glfwcompat ${PROJECT_NAME} zetaml)
target_include_directories(glfwcompat PRIVATE "${PROJECT_SOURCE_DIR}/deps/glfw/include/GLFW")

# ---
# debug: demonstrating the use of an OpenGL debug context

add_executable(debug "debug.c")
target_link_libraries(debug ${PROJECT_NAME})
