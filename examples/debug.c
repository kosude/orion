#include "oriongl.h"    // base graphics library
#include "orionwin.h"   // orionwin is used to handle the window for simplicity

// ORION GRAPHICS LIBRARY DEMO
// 2022 Jack Bennett

/*========================================================================*/
/* This example file demonstrates the use of an OpenGL debug context with */
/* Orion GL.                                                              */
/*========================================================================*/

#define GLVERSION 430

int main() {
    // A window is created, with the OPENGL_DEBUG_CONTEXT flag set to true.
    oriWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    oriWindow *window = oriCreateWindow(300, 200, "Orion debug context", GLVERSION, GLFW_OPENGL_CORE_PROFILE);

    // Orion is initialised.
    oriInitialise(GLVERSION);

    // The ORION_DEBUG_CONTEXT flag is set to true and some GL message suppression flags are set.
    oriSetFlag(ORION_DEBUG_CONTEXT, true);
    oriDebugFlags(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, GL_DONT_CARE, false, NULL, 0); // this function is optional; if it is not called, all GL messages will display.
    // (here, shader compiler debug messages are hidden as they pollute stdout otherwise)

    // Some functions are run under invalid circumstances in order to demonstrate OpenGL errors.
    glClear(GL_VERTEX_SHADER);          // GL_INVALID_VALUE
    glDrawArrays(GL_TRIANGLES, 0, 3);   // GL_INVALID_OPERATION

    while (!oriWindowShouldClose(window)) {
        // The window is cleared to a green colour.
        glClearColor(0.3, 1.0, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        oriSwapBuffers(window);
        oriPollEvents();
    }

    // The library and program are terminated.
    oriTerminate();
    return 0;
}
