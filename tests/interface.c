#include <stdio.h>

#include "oriongl.h"

int main() {
 	oriWindow *window = oriCreateWindow(640, 480, "Orion GLFW test", 460, GLFW_OPENGL_COMPAT_PROFILE);
	oriMakeContextCurrent(window);

	oriInitialiseGL(460);

	while (!oriWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		oriPollEvents();
		oriSwapBuffers(window);
	}

	oriTerminate();
	return 0;
}