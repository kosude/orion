/* *************************************************************************************** */
/* 						ORION GRAPHICS LIBRARY AND RENDERING ENGINE						   */
/* *************************************************************************************** */
/* Copyright (c) 2022 Jack Bennett														   */
/* --------------------------------------------------------------------------------------- */
/* THE  SOFTWARE IS  PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND, EXPRESS  OR IMPLIED, */
/* INCLUDING  BUT  NOT  LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS FOR  A */
/* PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN  NO EVENT SHALL  THE  AUTHORS  OR COPYRIGHT */
/* HOLDERS  BE  LIABLE  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF */
/* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR */
/* THE USE OR OTHER DEALINGS IN THE SOFTWARE.											   */
/* *************************************************************************************** */

#include "internal.h"
#include "oriongl.h"

#include <stdlib.h>
#include <math.h>

// ======================================================================================
// ***** 				   		 ORION PUBLIC STRUCTURES 							*****
// ======================================================================================

/**
 * @brief An Orion window structure.
 * @details This structure is simply an abstraction of @c GLFWwindow, part of the GLFW public API.
 * @sa <a href="https://www.glfw.org/docs/latest/window.html">GLFW window guide</a>
 *
 * @ingroup window
 */
typedef struct oriWindow {
	struct oriWindow *next;

	GLFWwindow *handle;
} oriWindow;

// ======================================================================================
// ***** 				   	  ORION WINDOW MANAGEMENT (ORIONWIN)					*****
// ======================================================================================

/**
 * @brief Allocate and initialise a GLFW window struct.
 * @details This structure is simply an abstraction of @c glfwCreateWindow, part of the GLFW public API.
 * The resulting window, however, will be automatically freed in a call to oriTerminate().
 * @sa <a href="https://www.glfw.org/docs/latest/window.html">GLFW window guide</a>
 *
 * @ingroup window
 */
oriWindow *oriCreateWindow(const unsigned int width, const unsigned int height, const char *title, const unsigned int version, const unsigned int profile) {
	// automatically initialise GLFW the first time a window is created.
	// this means GLFW isn't initialised if no window is created (and therefore Orion windows aren't being used)
	if (!_orion.glfwInitialised) {
		// initialise GLFW
		// errors will be handled by the appropriate error callback in _oriCallbacks.
		glfwInit();
		_orion.glfwInitialised = true;
	}

	unsigned int major = version / 100;
	unsigned int minor = (version / 10) % 10;

	// window hints based on given parameters
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
	
	// TODO: add monitor + share functionality
	GLFWwindow *rhandle = glfwCreateWindow(width, height, title, NULL, NULL);

	// reset to default window hints
	glfwDefaultWindowHints();

	if (!rhandle) {
		_orionThrowError(ORERR_GLFW_FAIL);
	}

	oriWindow *r;
	r->handle = rhandle;

	// link to global linked list (add to the start)
	r->next = _orion.windowListHead;
	_orion.windowListHead = r;

	return r;
}

/**
 * @brief Destroy and free memory for a given window.
 * 
 * @param window The window to free.
 */
void oriFreeWindow(oriWindow *window) {
	// unlink from global linked list
	oriWindow **prev = &_orion.windowListHead;
	while (*prev != window)
		*prev = (*prev)->next;
	*prev = window->next;

	glfwDestroyWindow(window->handle);
	free(window);
	window = NULL;
}

/**
 * @brief Return a pointer to the GLFW handle of the given oriWindow structure so that it can be used in GLFW functions.
 * @warning This function can be used to directly modify the GLFWwindow pointer; doing so can result in undefined behaviour!
 * 
 * @param window the window of which the handle will be returned.
 * 
 * @ingroup window
 */
GLFWwindow **oriWindowToGLFW(oriWindow *window) {
	return &(window->handle);
}