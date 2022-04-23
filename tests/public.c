#include "oriongl.h"
#include "orionwin.h"

int main() {
	oriWindow *mainWin = oriCreateWindow(480, 480, "Orion public interface test", 330, GLFW_OPENGL_CORE_PROFILE);
	oriSwapInterval(mainWin, 1);

	oriInitialise(330);

	while (!oriWindowShouldClose(mainWin)) {
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		oriSwapBuffers(mainWin);

		oriPollEvents();
	}

	oriTerminate();
	return 0;
}