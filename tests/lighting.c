#include "testkit/oriontk.h"

#include <zetaml/include/zetaml.h>
#include <stb_image/stb_image.h>

// Set any initialisation hints here. Run before anything else.
void preload() {
    oritk.windowTitle = "Orion lighting test";
}

// Initialise your test program in this functions. Run after window creation and library initialisation.
void initialise() {

}

// Execute any logic for updating and rendering here; this function is run every frame.
void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    oriSwapBuffers(oritk.window);
    oriPollEvents();
}

// Free any allocated memory here. Run just before library termination.
void clean() {

}
