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
 * @brief The function pointer type for window position callbacks.
 * @sa <a href="https://www.glfw.org/docs/latest/group__window.html#gabe287973a21a8f927cde4db06b8dcbe9">GLFWwindowposfun</a>
 * 
 * @ingroup window
 */
typedef void(* oriWindowPosCallback)(oriWindow *window, int xpos, int ypos);
/**
 * @brief The function pointer type for window size callbacks.
 * @sa <a href="https://www.glfw.org/docs/latest/group__window.html#gaec0282944bb810f6f3163ec02da90350">GLFWwindowsizefun</a>
 * 
 * @ingroup window
 */
typedef void(* oriWindowSizeCallback)(oriWindow *window, int width, int height);
/**
 * @brief The function pointer type for window close callbacks.
 * @sa <a href="https://www.glfw.org/docs/latest/group__window.html#gabf859b936d80961b7d39013a9694cc3e">GLFWwindowclosefun</a>
 * 
 * @ingroup window
 */
typedef void(* oriWindowCloseCallback)(oriWindow *window);
/**
 * @brief The function pointer type for window content refresh callbacks.
 * @sa <a href="https://www.glfw.org/docs/latest/group__window.html#ga431663a1427d2eb3a273bc398b6737b5">GLFWwindowrefreshfun</a>
 * 
 * @ingroup window
 */
typedef void(* oriWindowRefreshCallback)(oriWindow *window);
/**
 * @brief The function pointer type for window focus callbacks.
 * @sa <a href="https://www.glfw.org/docs/latest/group__window.html#gabc58c47e9d93f6eb1862d615c3680f46">GLFWwindowfocusfun</a>
 * 
 * @ingroup window
 */
typedef void(* oriWindowFocusCallback)(oriWindow *window, int focused);
/**
 * @brief The function pointer type for window iconify callbacks.
 * @sa <a href="https://www.glfw.org/docs/latest/group__window.html#ga35c658cccba236f26e7adee0e25f6a4f">GLFWwindowiconifyfun</a>
 * 
 * @ingroup window
 */
typedef void(* oriWindowIconifyCallback)(oriWindow *window, int iconified);
/**
 * @brief The function pointer type for window maximise callbacks.
 * @sa <a href="https://www.glfw.org/docs/latest/group__window.html#ga3017196fdaec33ac3e095765176c2a90">GLFWwindowmaximizefun</a>
 * 
 * @ingroup window
 */
typedef void(* oriWindowMaximiseCallback)(oriWindow *window, int maximised);
/**
 * @brief The function pointer type for framebuffer size callbacks.
 * @sa <a href="https://www.glfw.org/docs/latest/group__window.html#gae18026e294dde685ed2e5f759533144d">GLFWframebuffersizefun</a>
 * 
 * @ingroup window
 */
typedef void(* oriWindowFramebufferSizeCallback)(oriWindow *window, int width, int height);
/**
 * @brief The function pointer type for window content scale callbacks.
 * @sa <a href="https://www.glfw.org/docs/latest/group__window.html#ga77f288a2d04bb3c77c7d9615d08cf70e">GLFWwindowcontentscalefun</a>
 * 
 * @ingroup window
 */
typedef void(* oriWindowContentScaleCallback)(oriWindow *window, float xscale, float yscale);

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

/** @ingroup window */ void oriDefaultWindowHints();
/** @ingroup window */ void oriWindowHint(int hint, int value);
/** @ingroup window */ void oriWindowHintString(int hint, const char *value);
/** @ingroup window */ int 	oriWindowShouldClose(oriWindow *window);
/** @ingroup window */ void oriSetWindowShouldClose(oriWindow *window);
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
/** @ingroup window */ oriWindowPosCallback oriSetWindowPosCallback(oriWindow *window, oriWindowPosCallback callback);
/** @ingroup window */ oriWindowSizeCallback oriSetWindowSizeCallback(oriWindow *window, oriWindowSizeCallback callback);
/** @ingroup window */ oriWindowCloseCallback oriSetWindowCloseCallback(oriWindow *window, oriWindowCloseCallback callback);
/** @ingroup window */ oriWindowRefreshCallback oriSetWindowRefreshCallback(oriWindow *window, oriWindowRefreshCallback callback);
/** @ingroup window */ oriWindowFocusCallback oriSetWindowFocusCallback(oriWindow *window, oriWindowFocusCallback callback);
/** @ingroup window */ oriWindowIconifyCallback oriSetWindowIconifyCallback(oriWindow *window, oriWindowIconifyCallback callback);
/** @ingroup window */ oriWindowMaximiseCallback oriSetWindowMaximiseCallback(oriWindow *window, oriWindowMaximiseCallback callback);
/** @ingroup window */ oriWindowFramebufferSizeCallback oriSetWindowFramebufferSizeCallback(oriWindow *window, oriWindowFramebufferSizeCallback callback);
/** @ingroup window */ oriWindowContentScaleCallback oriSetWindowContentScaleCallback(oriWindow *window, oriWindowContentScaleCallback callback);
/** @ingroup window */ void oriPollEvents();
/** @ingroup window */ void oriWaitEvents();
/** @ingroup window */ void oriWaitEventsTimeout(double timeout);
/** @ingroup window */ void oriPostEmptyEvent();
/** @ingroup window */ void oriSwapBuffers(oriWindow *window);

#ifdef __cplusplus
}
#endif

#endif // include guard