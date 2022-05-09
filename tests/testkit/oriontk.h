/* *************************************************************************************** */
/*                  TEST KIT FOR DEVELOPMENT OF THE ORION GRAPHICS LIBRARY                 */
/* *************************************************************************************** */
/* Copyright (c) 2022 Jack Bennett                                                         */
/* --------------------------------------------------------------------------------------- */
/* THE  SOFTWARE IS  PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND, EXPRESS  OR IMPLIED, */
/* INCLUDING  BUT  NOT  LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS FOR  A */
/* PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN  NO EVENT SHALL  THE  AUTHORS  OR COPYRIGHT */
/* HOLDERS  BE  LIABLE  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF */
/* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR */
/* THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                              */
/* *************************************************************************************** */

// NOTE: This is NOT an API or a library of any kind. Its functionality may be limited, but
//       it is NOT part of the core Orion public interface, and so it will only have what is
//       necessary for the project's tests. This header only serves to simplify and better
//       automate the creation of Orion test executables. It is only intended to be used in
//       the aforementioned tests.

#pragma once
#ifndef __ORION_TESTKIT_GL_H
#define __ORION_TESTKIT_GL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "oriongl.h"
#include "orionwin.h"

// Some macros can be defined to customise the test:
//      ORION_TK_DEBUG_CONTEXT :: use an OpenGL debug context (TRUE/false).

#ifndef ORION_TK_DEBUG_CONTEXT
#   define ORION_TK_DEBUG_CONTEXT true
#endif

// ======================================================================================
// *****                              PUBLIC CONSTANTS                              *****
// ======================================================================================

const float squareVertices[] = {
    // vertex positions     // Colours      // Tex coords
    -0.5f, -0.5f, 0.0f,     1, 0, 0, 1,     0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,     0, 1, 0, 1,     1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,     0, 0, 1, 1,     0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,     1, 1, 0, 1,     1.0f, 1.0f
};
const unsigned int squareIndices[] = {
    0, 1, 2,
    3, 1, 2
};

const float cubeVertices[] = {
	// vertices				// tex coords	// normals
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,		 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,		 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		 0.0f,  0.0f, -1.0f,
													
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		 0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,		 0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,		 0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,		 0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,		 0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		 0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		-1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		-1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		-1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,		 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,		 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,		 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,		 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,		 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,		 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,		 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,		 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,		 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,		 0.0f,  1.0f,  0.0f
};

// ======================================================================================
// *****                              PUBLIC VARIABLES                              *****
// ======================================================================================

typedef struct oritkState {
    // The width of the window.
    unsigned int windowWidth;
    // The height of the window.
    unsigned int windowHeight;
    // The title of the window.
    char *windowTitle;

    // Time between this frame and the last frame.
    float windowDeltaTime;
    // The absolute GLFW time of the last frame.
    float windowLastFrameTime;

    // The GL version to use.
    unsigned int glVersion;
    // The GL profile to use.
    unsigned int glProfile;

    // The test's window.
    oriWindow *window;
} oritkState;

oritkState oritk;

// ======================================================================================
// *****                           USER-DEFINED FUNCTIONS                           *****
// ======================================================================================

// Set any initialisation hints here. Run before anything else.
void preload();

// Initialise your test program in this functions. Run after window creation and library initialisation.
void initialise();

// Execute any logic for updating and rendering here; this function is run every frame.
void render();

// Free any allocated memory here. Run just before library termination.
void clean();

// ======================================================================================
// *****                            INLINE MAIN FUNCTION                            *****
// ======================================================================================
// The main() function is declared here to simplify tests

int main() {
    preload();

    oriInitialise(oritk.glVersion);

    oriWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    oritk.window = oriCreateWindow(oritk.windowWidth, oritk.windowHeight, oritk.windowTitle, oritk.glVersion, oritk.glProfile);

    #if ORION_TK_DEBUG_CONTEXT
        oriSetFlag(ORION_DEBUG_CONTEXT, true);
        oriDebugFlags(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, GL_DONT_CARE, false, NULL, 0);
    #endif

    initialise();

    while (!oriWindowShouldClose(oritk.window)) {
        double currentFrameTime = glfwGetTime();
        oritk.windowDeltaTime = currentFrameTime - oritk.windowLastFrameTime;
        oritk.windowLastFrameTime = currentFrameTime;

        render();
    }

    clean();

    oriTerminate();

    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
