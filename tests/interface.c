#include <stdio.h>

#include "oriongl.h"

int main() {
 	oriWindow *window = oriCreateWindow(640, 480, "Orion GLFW test", 460, GLFW_OPENGL_COMPAT_PROFILE);

	while (!oriWindowShouldClose(window)) {
		oriPollEvents();
		oriSwapBuffers(window);
	}

	oriTerminate();
	return 0;
}