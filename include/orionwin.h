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

/**
 * @file orionwin.h
 * @brief The main Orionwin include file; abstractions of GLFW functions for the oriWindow structure.
 * @author Jack Bennett
 * 
 */

#pragma once
#ifndef __ORIONWIN_H
#define __ORIONWIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <glfw/include/GLFW/glfw3.h>

// ======================================================================================
// ***** 				   		 ORIONWIN PUBLIC STRUCTURES							*****
// ======================================================================================

/**
 * @brief Generic function pointer used for returning client API function pointers without forcing a cast from a regular pointer.
 * 
 * @sa <a href="https://www.glfw.org/docs/latest/group__context.html#ga3d47c2d2fbe0be9c505d0e04e91a133c">GLFW context reference/GLFWglproc</a>
 * 
 * @ingroup window
 */
typedef GLFWglproc oriGLProcAddress;

/**
 * @brief An Orion window structure.
 * @details This structure is simply an abstraction of @c GLFWwindow, part of the GLFW public API.
 * @sa <a href="https://www.glfw.org/docs/latest/window.html">GLFW window guide</a>
 *
 * @note All instances of oriWindow will be cleared with oriTerminate().
 * 
 * @ingroup window
 */
typedef struct oriWindow oriWindow;

// ======================================================================================
// ***** 				   	  ORION WINDOW MANAGEMENT (ORIONWIN)					*****
// ======================================================================================

/**
 * @brief Allocate and initialise a GLFW window struct, make its context current, and load OpenGL for its context.
 * @details This structure is simply an abstraction of @c glfwCreateWindow, part of the GLFW public API.
 * The resulting window, however, will be automatically freed in a call to oriTerminate().
 * 
 * GLFW will be initialised automatically in the first window creation.
 * 
 * @sa <a href="https://www.glfw.org/docs/latest/window.html">GLFW window guide</a>
 * 
 * @todo GLFW @c monitor and @c share parameters are not yet implemented.
 *
 * @ingroup window
 */
oriWindow *oriCreateWindow(const unsigned int width, const unsigned int height, const char *title, const unsigned int version, const unsigned int profile);

/**
 * @brief Destroy and free memory for a given window.
 * 
 * @note This will automatically be done in a call to oriTerminate().
 * 
 * @param window the window to free.
 * 
 * @ingroup window
 */
void oriFreeWindow(oriWindow *window);

/**
 * @brief Return a pointer to the GLFW handle of the given oriWindow structure so that it can be used in GLFW functions.
 * @warning This function can be used to directly modify the GLFWwindow pointer; doing so can result in undefined behaviour!
 * 
 * @param window the window of which the handle will be returned.
 * 
 * @ingroup window
 */
const GLFWwindow *oriWindowHandle(const oriWindow *window);

// ======================================================================================
// ***** 				   	  		ORION GLFW ABSTRACTIONS							*****
// ======================================================================================

// ======================================================================================
// ***** 				   	 from the GLFW INITIALISATION REFERENCE					*****
// ======================================================================================
// (https://www.glfw.org/docs/latest/group__init.html)

/**
 * @brief Initialise the GLFW library for use in Orion abstractions of GLFW functionality.
 * @details This function is @b automatically @b called when the first oriWindow is created.
 * Calling it after creating the first window won't do any harm; the function will just
 * return early. The only real reason for this abstraction existing is for the ability to re-initialise
 * GLFW without terminating Orion.
 * 
 * @note The result of @c glfwInit() will automatically be checked and the program will halt (with a
 * debug message) should GLFW fail to initialise.
 * 
 * @sa <a href="https://www.glfw.org/docs/latest/group__init.html#ga317aac130a235ab08c6db0834907d85e">GLFW reference/glfwInit()</a>
 * @sa oriTerminateGLFW()
 * @sa oriGLFWInitHint()
 * 
 * @ingroup window
 */
void oriInitGLFW();

/**
 * @brief Terminate the GLFW library.
 * @details @b You @b will @b rarely @b need @b to @b do @b this. GLFW is automatically terminated
 * when you call oriTerminate(). But this function is still here in case you want to re-initialise
 * GLFW without terminating Orion.
 * 
 * @sa <a href="https://www.glfw.org/docs/latest/group__init.html#gaaae48c0a18607ea4a4ba951d939f0901">GLFW reference/glfwTerminate()</a>
 * @sa oriInitGLFW()
 * 
 * @ingroup window 
 */
void oriTerminateGLFW();

/**
 * @brief Sets hints for the next initialization of GLFW.
 * @details GLFW is initialised either with a call to oriInitGLFW or by creating an oriWindow.
 * 
 * @param hint the hint to set
 * @param value the value of the init hint.
 * 
 * @sa <a href="https://www.glfw.org/docs/latest/group__init.html#ga110fd1d3f0412822b4f1908c026f724a">GLFW reference/glfwInitHint()</a>
 * @sa oriInitGLFW()
 * 
 * @ingroup window
 */
void oriGLFWInitHint(int hint, int value);
 
/** @ingroup window */ void oriGetGLFWVersion(int *major, int *minor, int *rev);
/** @ingroup window */ const char *oriGetGLFWVersionString();
/** @ingroup window */ int oriGetGLFWError(const char **description);

// ======================================================================================
// ***** 				   	  	from the GLFW WINDOW REFERENCE						*****
// ======================================================================================
// (https://www.glfw.org/docs/latest/group__window.html)

/** @ingroup window */ void oriDefaultWindowHints();
/** @ingroup window */ void oriWindowHint(int hint, int value);
/** @ingroup window */ void oriWindowHintString(int hint, const char *value);
/** @ingroup window */ int 	oriWindowShouldClose(oriWindow *window);
/** @ingroup window */ void oriSetWindowShouldClose(oriWindow *window, int value);
/** @ingroup window */ void oriSetWindowTitle(oriWindow *window, const char *title);
						// 	oriSetWindowIcon()
/** @ingroup window */ void oriGetWindowPos(oriWindow *window, int *xpos, int *ypos);
/** @ingroup window */ void oriSetWindowPos(oriWindow *window, int xpos, int ypos);
/** @ingroup window */ void oriGetWindowSize(oriWindow *window, int *width, int *height);
/** @ingroup window */ void oriSetWindowSizeLimits(oriWindow *window, int minwidth, int minheight, int maxwidth, int maxheight);
/** @ingroup window */ void oriSetWindowAspectRatio(oriWindow *window, int numer, int denom);
/** @ingroup window */ void oriSetWindowSize(oriWindow *window, int width, int height);
/** @ingroup window */ void oriGetFramebufferSize(oriWindow *window, int *width, int *height);
/** @ingroup window */ void oriGetWindowFrameSize(oriWindow *window, int *left, int *top, int *right, int *bottom);
/** @ingroup window */ void oriGetWindowContentScale(oriWindow *window, float *xscale, float *yscale);
/** @ingroup window */ float oriGetWindowOpacity(oriWindow *window);
/** @ingroup window */ void oriSetWindowOpacity(oriWindow *window, float opacity);
/** @ingroup window */ void oriIconifyWindow(oriWindow *window);
/** @ingroup window */ void oriRestoreWindow(oriWindow *window);
/** @ingroup window */ void oriMaximiseWindow(oriWindow *window);
/** @ingroup window */ void oriShowWindow(oriWindow *window);
/** @ingroup window */ void oriHideWindow(oriWindow *window);
/** @ingroup window */ void oriFocusWindow(oriWindow *window);
/** @ingroup window */ void oriRequestWindowAttention(oriWindow *window);
						// 	oriGetWindowMonitor()
						// 	oriSetWindowMonitor()
/** @ingroup window */ int oriGetWindowAttrib(oriWindow *window, int attrib);
/** @ingroup window */ void oriSetWindowAttrib(oriWindow *window, int attrib, int value);
/** @ingroup window */ void oriSetWindowUserPointer(oriWindow *window, void *pointer);
/** @ingroup window */ void *oriGetWindowUserPointer(oriWindow *window);
/** @ingroup window */ GLFWwindowposfun oriSetWindowPosCallback(oriWindow *window, GLFWwindowposfun callback);
/** @ingroup window */ GLFWwindowsizefun oriSetWindowSizeCallback(oriWindow *window, GLFWwindowsizefun callback);
/** @ingroup window */ GLFWwindowclosefun oriSetWindowCloseCallback(oriWindow *window, GLFWwindowclosefun callback);
/** @ingroup window */ GLFWwindowrefreshfun oriSetWindowRefreshCallback(oriWindow *window, GLFWwindowrefreshfun callback);
/** @ingroup window */ GLFWwindowfocusfun oriSetWindowFocusCallback(oriWindow *window, GLFWwindowfocusfun callback);
/** @ingroup window */ GLFWwindowiconifyfun oriSetWindowIconifyCallback(oriWindow *window, GLFWwindowiconifyfun callback);
/** @ingroup window */ GLFWwindowmaximizefun oriSetWindowMaximiseCallback(oriWindow *window, GLFWwindowmaximizefun callback);
/** @ingroup window */ GLFWframebuffersizefun oriSetFramebufferSizeCallback(oriWindow *window, GLFWframebuffersizefun callback);
/** @ingroup window */ GLFWwindowcontentscalefun oriSetWindowContentScaleCallback(oriWindow *window, GLFWwindowcontentscalefun callback);
/** @ingroup window */ void oriPollEvents();
/** @ingroup window */ void oriWaitEvents();
/** @ingroup window */ void oriWaitEventsTimeout(double timeout);
/** @ingroup window */ void oriPostEmptyEvent();
/** @ingroup window */ void oriSwapBuffers(oriWindow *window);

// ======================================================================================
// ***** 				   	  	from the GLFW CONTEXT REFERENCE						*****
// ======================================================================================
// (https://www.glfw.org/docs/latest/group__context.html)

/** @ingroup window */ void oriMakeContextCurrent(oriWindow *window);
/** @ingroup window */ oriWindow *oriGetCurrentContext();
/** @ingroup window */ void oriSwapInterval(oriWindow *window, int interval);
/** @ingroup window */ int oriExtensionSupported(oriWindow *window, const char *extension);
/** @ingroup window */ oriGLProcAddress oriGetGLProcAddress(oriWindow *window, const char *procname);

#ifdef __cplusplus
}
#endif

#endif // include guard