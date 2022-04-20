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
 * @file oriongl.h
 * @brief The main Orion include file.
 * @author Jack Bennett
 * 
 */

#pragma once
#ifndef __ORIONGL_H
#define __ORIONGL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ORION_INCLUDE_NOTHING
#	include <glad/4.6/glad.h>
#	include <glfw/include/GLFW/glfw3.h>
#endif

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
typedef struct oriWindow oriWindow;

// ======================================================================================
// ***** 				   ORION PUBLIC INITIALISATION FUNCTIONS 					*****
// ======================================================================================

/**
 * @brief Initialise the global (internal) Orion state.
 * 
 * @param version the version of OpenGL that is to be used.
 * @param profile the OpenGL profile to use.
 * 
 * @ingroup meta
 */
void oriInitialise(const unsigned int version, const unsigned int profile);

/**
 * @brief Terminate the Orion library. All Orion objects that were allocated will be freed.
 * 
 * @ingroup meta
 */
void oriTerminate();

// ======================================================================================
// ***** 				   	  ORION WINDOW MANAGEMENT (ORIONWIN)					*****
// ======================================================================================

/**
 * @brief Allocate and initialise a GLFW window struct.
 * @details This structure is simply an abstraction of @c glfwCreateWindow, part of the GLFW public API.
 * The resulting window, however, will be automatically freed in a call to oriTerminate().
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
 * @param window The window to free.
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
GLFWwindow **oriWindowToGLFW(oriWindow *window);

// ======================================================================================
// ***** 				   	  		ORION GLFW ABSTRACTIONS							*****
// ======================================================================================

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

#ifdef __cplusplus
}
#endif

#endif // include guard