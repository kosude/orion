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
#include "orionwin.h"

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
 * @brief Allocate and initialise a GLFW window struct, make its context current, and load OpenGL for its context.
 * @details This structure is simply an abstraction of @c glfwCreateWindow, part of the GLFW public API.
 * The resulting window, however, will be automatically freed in a call to oriTerminate().
 * 
 * @sa <a href="https://www.glfw.org/docs/latest/window.html">GLFW window guide</a>
 * 
 * @todo GLFW @c monitor and @c share parameters are not yet implemented.
 *
 * @ingroup window
 */
oriWindow *oriCreateWindow(const unsigned int width, const unsigned int height, const char *title, const unsigned int version, const unsigned int profile) {
	// automatically initialise GLFW the first time a window is created.
	// this means GLFW isn't initialised if no window is created (and therefore Orion windows aren't being used)
	if (!_orion.glfwInitialised) {
		_orionInitGLFW();
	}

	unsigned int major = version / 100;
	unsigned int minor = (version / 10) % 10;

	// window hints based on given parameters
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, profile);

	if (_orion.debug) {
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	}
	
	// TODO: add monitor + share functionality
	GLFWwindow *rhandle = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!rhandle) {
		_orionThrowError(ORERR_GLFW_FAIL);
	}

	oriWindow *r = malloc(sizeof(oriWindow));
	r->handle = rhandle;

	// link to global linked list (add to the start)
	r->next = _orion.windowListHead;
	_orion.windowListHead = r;

	// load OpenGL after creation
	oriMakeContextCurrent(r);
	oriLoadGL((GLADloadproc) glfwGetProcAddress);

	return r;
}

/**
 * @brief Destroy and free memory for a given window.
 * 
 * @param window The window to free.
 */
void oriFreeWindow(oriWindow *window) {
	// unlink from global linked list
	oriWindow *current = _orion.windowListHead;
	while (current != window)
		current = current->next;
	_orion.windowListHead = window->next;

	glfwDestroyWindow(window->handle);
	free(window);
	window = NULL;
}

/**
 * @brief Return a pointer to the GLFW handle of the given oriWindow structure so that it can be used in GLFW functions.
 * @warning This function can be used to directly modify the GLFWwindow pointer; doing so can (or rather @b will) result in undefined behaviour!
 * 
 * @param window the window of which the handle will be returned.
 * 
 * @ingroup window
 */
GLFWwindow **oriWindowToGLFW(oriWindow *window) {
	return &(window->handle);
}

// ======================================================================================
// ***** 				   	  		ORION GLFW ABSTRACTIONS							*****
// ======================================================================================

#define _ori_initGLFWIfReq() \
{\
	if (!_orion.glfwInitialised) {\
		_orionInitGLFW();\
	}\
}

/** @ingroup window */ void oriDefaultWindowHints() {
	_ori_initGLFWIfReq();
	glfwDefaultWindowHints();
}
/** @ingroup window */ void oriWindowHint(int hint, int value) {
	_ori_initGLFWIfReq();
	glfwWindowHint(hint, value);
}
/** @ingroup window */ void oriWindowHintString(int hint, const char *value) {
	_ori_initGLFWIfReq();
	glfwWindowHintString(hint, value);
}
/** @ingroup window */ int oriWindowShouldClose(oriWindow *window) {
	_ori_initGLFWIfReq();
	return glfwWindowShouldClose(window->handle);
}
/** @ingroup window */ void oriSetWindowShouldClose(oriWindow *window, int value) {
	_ori_initGLFWIfReq();
	glfwSetWindowShouldClose(window->handle, value);
}
/** @ingroup window */ void oriSetWindowTitle(oriWindow *window, const char *title) {
	_ori_initGLFWIfReq();
	glfwSetWindowTitle(window->handle, title);
}
/** @ingroup window */ void oriGetWindowPos(oriWindow *window, int *xpos, int *ypos) {
	_ori_initGLFWIfReq();
	glfwGetWindowPos(window->handle, xpos, ypos);
}
/** @ingroup window */ void oriSetWindowPos(oriWindow *window, int xpos, int ypos) {
	_ori_initGLFWIfReq();
	glfwSetWindowPos(window->handle, xpos, ypos);
}
/** @ingroup window */ void oriGetWindowSize(oriWindow *window, int *width, int *height) {
	_ori_initGLFWIfReq();
	glfwGetWindowSize(window->handle, width, height);
}
/** @ingroup window */ void oriSetWindowSizeLimits(oriWindow *window, int minwidth, int minheight, int maxwidth, int maxheight) {
	_ori_initGLFWIfReq();
	glfwSetWindowSizeLimits(window->handle, minwidth, minheight, maxwidth, maxheight);
}
/** @ingroup window */ void oriSetWindowAspectRatio(oriWindow *window, int numer, int denom) {
	_ori_initGLFWIfReq();
	glfwSetWindowAspectRatio(window->handle, numer, denom);
}
/** @ingroup window */ void oriSetWindowSize(oriWindow *window, int width, int height) {
	_ori_initGLFWIfReq();
	glfwSetWindowSize(window->handle, width, height);
}
/** @ingroup window */ void oriGetFramebufferSize(oriWindow *window, int *width, int *height) {
	_ori_initGLFWIfReq();
	glfwGetFramebufferSize(window->handle, width, height);
}
/** @ingroup window */ void oriGetWindowFrameSize(oriWindow *window, int *left, int *top, int *right, int *bottom) {
	_ori_initGLFWIfReq();
	glfwGetWindowFrameSize(window->handle, left, top, right, bottom);
}
/** @ingroup window */ void oriGetWindowContentScale(oriWindow *window, float *xscale, float *yscale) {
	_ori_initGLFWIfReq();
	glfwGetWindowContentScale(window->handle, xscale, yscale);
}
/** @ingroup window */ float oriGetWindowOpacity(oriWindow *window) {
	_ori_initGLFWIfReq();
	return glfwGetWindowOpacity(window->handle);
}
/** @ingroup window */ void oriSetWindowOpacity(oriWindow *window, float opacity) {
	_ori_initGLFWIfReq();
	glfwSetWindowOpacity(window->handle, opacity);
}
/** @ingroup window */ void oriIconifyWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwIconifyWindow(window->handle);
}
/** @ingroup window */ void oriRestoreWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwRestoreWindow(window->handle);
}
/** @ingroup window */ void oriMaximiseWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwMaximizeWindow(window->handle);
}
/** @ingroup window */ void oriShowWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwShowWindow(window->handle);
}
/** @ingroup window */ void oriHideWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwHideWindow(window->handle);
}
/** @ingroup window */ void oriFocusWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwFocusWindow(window->handle);
}
/** @ingroup window */ void oriRequestWindowAttention(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwRequestWindowAttention(window->handle);
}
/** @ingroup window */ int oriGetWindowAttrib(oriWindow *window, int attrib) {
	_ori_initGLFWIfReq();
	return glfwGetWindowAttrib(window->handle, attrib);
}
/** @ingroup window */ void oriSetWindowAttrib(oriWindow *window, int attrib, int value) {
	_ori_initGLFWIfReq();
	glfwSetWindowAttrib(window->handle, attrib, value);
}
/** @ingroup window */ void oriSetWindowUserPointer(oriWindow *window, void *pointer) {
	_ori_initGLFWIfReq();
	glfwSetWindowUserPointer(window->handle, pointer);
}
/** @ingroup window */ void *oriGetWindowUserPointer(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwGetWindowUserPointer(window->handle);
}
/** @ingroup window */ GLFWwindowposfun oriSetWindowPosCallback(oriWindow *window, GLFWwindowposfun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowPosCallback(window->handle, callback);
}
/** @ingroup window */ GLFWwindowsizefun oriSetWindowSizeCallback(oriWindow *window, GLFWwindowsizefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowSizeCallback(window->handle, callback);
}
/** @ingroup window */ GLFWwindowclosefun oriSetWindowCloseCallback(oriWindow *window, GLFWwindowclosefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowCloseCallback(window->handle, callback);
}
/** @ingroup window */ GLFWwindowrefreshfun oriSetWindowRefreshCallback(oriWindow *window, GLFWwindowrefreshfun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowRefreshCallback(window->handle, callback);
}
/** @ingroup window */ GLFWwindowfocusfun oriSetWindowFocusCallback(oriWindow *window, GLFWwindowfocusfun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowFocusCallback(window->handle, callback);
}
/** @ingroup window */ GLFWwindowiconifyfun oriSetWindowIconifyCallback(oriWindow *window, GLFWwindowiconifyfun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowIconifyCallback(window->handle, callback);
}
/** @ingroup window */ GLFWwindowmaximizefun oriSetWindowMaximiseCallback(oriWindow *window, GLFWwindowmaximizefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowMaximizeCallback(window->handle, callback);
}
/** @ingroup window */ GLFWframebuffersizefun oriSetFramebufferSizeCallback(oriWindow *window, GLFWframebuffersizefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetFramebufferSizeCallback(window->handle, callback);
}
/** @ingroup window */ GLFWwindowcontentscalefun oriSetWindowContentScaleCallback(oriWindow *window, GLFWwindowcontentscalefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowContentScaleCallback(window->handle, callback);
}
/** @ingroup window */ void oriPollEvents() {
	_ori_initGLFWIfReq();
	glfwPollEvents();
}
/** @ingroup window */ void oriWaitEvents() {
	_ori_initGLFWIfReq();
	glfwWaitEvents();
}
/** @ingroup window */ void oriWaitEventsTimeout(double timeout) {
	_ori_initGLFWIfReq();
	glfwWaitEventsTimeout(timeout);
}
/** @ingroup window */ void oriPostEmptyEvent() {
	_ori_initGLFWIfReq();
	glfwPostEmptyEvent();
}
/** @ingroup window */ void oriSwapBuffers(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwSwapBuffers(window->handle);
}
/** @ingroup window */ void oriMakeContextCurrent(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwMakeContextCurrent(window->handle);
}