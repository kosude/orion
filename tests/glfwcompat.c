#include "oriongl.h"
#include <glfw3.h>

int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);

	GLFWwindow *window = glfwCreateWindow(300, 300, "Orion with GLFW", NULL, NULL);
	glfwMakeContextCurrent(window);

	oriLoadGL((GLADloadproc) glfwGetProcAddress);
	glfwSwapInterval(1);

	oriInitialise(460);
	oriEnableDebugContext(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, GL_DONT_CARE, false, NULL, 0);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.9, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	oriTerminate();
	glfwTerminate();
	return 0;
}