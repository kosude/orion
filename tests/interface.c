#include <stdio.h>

#include "oriongl.h"

int main() {
 	oriWindow *window = oriCreateWindow(640, 480, "Orion GLFW test", 460, GLFW_OPENGL_COMPAT_PROFILE);

	oriFreeWindow(window);

	glfwTerminate();
	oriTerminate();
	return 0;
}