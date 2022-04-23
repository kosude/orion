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
const GLFWwindow *oriWindowHandle(const oriWindow *window) {
	return window->handle;
}

// ======================================================================================
// ***** 				   	  		ORION GLFW ABSTRACTIONS							*****
// ======================================================================================

#define _ori_initGLFWIfReq()\
{\
	if (!_orion.glfwInitialised) {\
		_orionInitGLFW();\
	}\
}

#define _ori_GLFWContextChangeHelper(win, func)\
{\
	GLFWwindow *prev = glfwGetCurrentContext();\
	glfwMakeContextCurrent(win->handle);\
	func;\
	glfwMakeContextCurrent(prev);\
}

#define _ori_GLFWContextChangeHelper_r(win, func, type)\
{\
	GLFWwindow *prev = glfwGetCurrentContext();\
	glfwMakeContextCurrent(win->handle);\
	type r = func;\
	glfwMakeContextCurrent(prev);\
	return r;\
}

// ======================================================================================
// ***** 				   	 from the GLFW INITIALISATION REFERENCE					*****
// ======================================================================================
// (https://www.glfw.org/docs/latest/group__init.html)

void oriInitGLFW() {
	if (_orion.glfwInitialised) {
		return;
	}

	_orionInitGLFW();
}
void oriTerminateGLFW() {
	glfwTerminate();
	_orion.glfwInitialised = false;
}
void oriGLFWInitHint(int hint, int value) {
	glfwSetErrorCallback(_oriCallbacks.glfwErrorCallback);
	glfwInitHint(hint, value);
}
void oriGetGLFWVersion(int *major, int *minor, int *rev) {
	glfwGetVersion(major, minor, rev);
}
const char *oriGetGLFWVersionString() {
	return glfwGetVersionString();
}
int oriGetGLFWError(const char **description) {
	return glfwGetError(description);
}

// ======================================================================================
// ***** 				   	  	from the GLFW WINDOW REFERENCE						*****
// ======================================================================================
// (https://www.glfw.org/docs/latest/group__window.html)

void oriDefaultWindowHints() {
	_ori_initGLFWIfReq();
	glfwDefaultWindowHints();
}
void oriWindowHint(int hint, int value) {
	_ori_initGLFWIfReq();
	glfwWindowHint(hint, value);
}
void oriWindowHintString(int hint, const char *value) {
	_ori_initGLFWIfReq();
	glfwWindowHintString(hint, value);
}
int oriWindowShouldClose(oriWindow *window) {
	_ori_initGLFWIfReq();
	return glfwWindowShouldClose(window->handle);
}
void oriSetWindowShouldClose(oriWindow *window, int value) {
	_ori_initGLFWIfReq();
	glfwSetWindowShouldClose(window->handle, value);
}
void oriSetWindowTitle(oriWindow *window, const char *title) {
	_ori_initGLFWIfReq();
	glfwSetWindowTitle(window->handle, title);
}
void oriGetWindowPos(oriWindow *window, int *xpos, int *ypos) {
	_ori_initGLFWIfReq();
	glfwGetWindowPos(window->handle, xpos, ypos);
}
void oriSetWindowPos(oriWindow *window, int xpos, int ypos) {
	_ori_initGLFWIfReq();
	glfwSetWindowPos(window->handle, xpos, ypos);
}
void oriGetWindowSize(oriWindow *window, int *width, int *height) {
	_ori_initGLFWIfReq();
	glfwGetWindowSize(window->handle, width, height);
}
void oriSetWindowSizeLimits(oriWindow *window, int minwidth, int minheight, int maxwidth, int maxheight) {
	_ori_initGLFWIfReq();
	glfwSetWindowSizeLimits(window->handle, minwidth, minheight, maxwidth, maxheight);
}
void oriSetWindowAspectRatio(oriWindow *window, int numer, int denom) {
	_ori_initGLFWIfReq();
	glfwSetWindowAspectRatio(window->handle, numer, denom);
}
void oriSetWindowSize(oriWindow *window, int width, int height) {
	_ori_initGLFWIfReq();
	glfwSetWindowSize(window->handle, width, height);
}
void oriGetFramebufferSize(oriWindow *window, int *width, int *height) {
	_ori_initGLFWIfReq();
	glfwGetFramebufferSize(window->handle, width, height);
}
void oriGetWindowFrameSize(oriWindow *window, int *left, int *top, int *right, int *bottom) {
	_ori_initGLFWIfReq();
	glfwGetWindowFrameSize(window->handle, left, top, right, bottom);
}
void oriGetWindowContentScale(oriWindow *window, float *xscale, float *yscale) {
	_ori_initGLFWIfReq();
	glfwGetWindowContentScale(window->handle, xscale, yscale);
}
float oriGetWindowOpacity(oriWindow *window) {
	_ori_initGLFWIfReq();
	return glfwGetWindowOpacity(window->handle);
}
void oriSetWindowOpacity(oriWindow *window, float opacity) {
	_ori_initGLFWIfReq();
	glfwSetWindowOpacity(window->handle, opacity);
}
void oriIconifyWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwIconifyWindow(window->handle);
}
void oriRestoreWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwRestoreWindow(window->handle);
}
void oriMaximiseWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwMaximizeWindow(window->handle);
}
void oriShowWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwShowWindow(window->handle);
}
void oriHideWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwHideWindow(window->handle);
}
void oriFocusWindow(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwFocusWindow(window->handle);
}
void oriRequestWindowAttention(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwRequestWindowAttention(window->handle);
}
int oriGetWindowAttrib(oriWindow *window, int attrib) {
	_ori_initGLFWIfReq();
	return glfwGetWindowAttrib(window->handle, attrib);
}
void oriSetWindowAttrib(oriWindow *window, int attrib, int value) {
	_ori_initGLFWIfReq();
	glfwSetWindowAttrib(window->handle, attrib, value);
}
void oriSetWindowUserPointer(oriWindow *window, void *pointer) {
	_ori_initGLFWIfReq();
	glfwSetWindowUserPointer(window->handle, pointer);
}
void *oriGetWindowUserPointer(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwGetWindowUserPointer(window->handle);
}
GLFWwindowposfun oriSetWindowPosCallback(oriWindow *window, GLFWwindowposfun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowPosCallback(window->handle, callback);
}
GLFWwindowsizefun oriSetWindowSizeCallback(oriWindow *window, GLFWwindowsizefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowSizeCallback(window->handle, callback);
}
GLFWwindowclosefun oriSetWindowCloseCallback(oriWindow *window, GLFWwindowclosefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowCloseCallback(window->handle, callback);
}
GLFWwindowrefreshfun oriSetWindowRefreshCallback(oriWindow *window, GLFWwindowrefreshfun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowRefreshCallback(window->handle, callback);
}
GLFWwindowfocusfun oriSetWindowFocusCallback(oriWindow *window, GLFWwindowfocusfun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowFocusCallback(window->handle, callback);
}
GLFWwindowiconifyfun oriSetWindowIconifyCallback(oriWindow *window, GLFWwindowiconifyfun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowIconifyCallback(window->handle, callback);
}
GLFWwindowmaximizefun oriSetWindowMaximiseCallback(oriWindow *window, GLFWwindowmaximizefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowMaximizeCallback(window->handle, callback);
}
GLFWframebuffersizefun oriSetFramebufferSizeCallback(oriWindow *window, GLFWframebuffersizefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetFramebufferSizeCallback(window->handle, callback);
}
GLFWwindowcontentscalefun oriSetWindowContentScaleCallback(oriWindow *window, GLFWwindowcontentscalefun callback) {
	_ori_initGLFWIfReq();
	return glfwSetWindowContentScaleCallback(window->handle, callback);
}
void oriPollEvents() {
	_ori_initGLFWIfReq();
	glfwPollEvents();
}
void oriWaitEvents() {
	_ori_initGLFWIfReq();
	glfwWaitEvents();
}
void oriWaitEventsTimeout(double timeout) {
	_ori_initGLFWIfReq();
	glfwWaitEventsTimeout(timeout);
}
void oriPostEmptyEvent() {
	_ori_initGLFWIfReq();
	glfwPostEmptyEvent();
}
void oriSwapBuffers(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwSwapBuffers(window->handle);
}

// ======================================================================================
// ***** 				   	  	from the GLFW CONTEXT REFERENCE						*****
// ======================================================================================
// (https://www.glfw.org/docs/latest/group__context.html)

void oriMakeContextCurrent(oriWindow *window) {
	_ori_initGLFWIfReq();
	glfwMakeContextCurrent(window->handle);
}
oriWindow *oriGetCurrentContext() {
	_ori_initGLFWIfReq();

	oriWindow *current = _orion.windowListHead;
	while (current && current->handle != glfwGetCurrentContext()) {
		current = current->next;
	}
	
	// if no window is found with the handle == current context (no window context is current), NULL is returned.
	// this mirrors the behaviour of the GLFW function, glfwGetCurrentContext().
	return (current) ? current : NULL;
	
}
void oriSwapInterval(oriWindow *window, int interval) {
	_ori_initGLFWIfReq();
	_ori_GLFWContextChangeHelper(window, glfwSwapInterval(interval));

}
int oriExtensionSupported(oriWindow *window, const char *extension) {
	_ori_initGLFWIfReq();
	_ori_GLFWContextChangeHelper_r(window, glfwExtensionSupported(extension), int);
}
oriGLProcAddress oriGetGLProcAddress(oriWindow *window, const char *procname) {
	_ori_initGLFWIfReq();
	_ori_GLFWContextChangeHelper_r(window, glfwGetProcAddress(procname), oriGLProcAddress);
}