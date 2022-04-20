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

#pragma once
#ifndef __ORI_INTERNAL_H
#define __ORI_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "oriongl.h"
#include "orionwin.h"
#include <stdbool.h>
#include <signal.h>

#ifdef SIGTRAP
#	define __debugbreak raise(SIGTRAP)
#else
#	define __debugbreak raise(SIGABRT)
#endif

// ======================================================================================
// ***** 				   		 ORION INTERNAL DATA TYPES 							*****
// ======================================================================================

/**
 * @brief Structure to store global mutable data.
 * 
 */
typedef struct _orionState {
	bool initialised;
	bool glfwInitialised; // this is only set to true if Orion/GLFW functions are used. Other windowing libraries can also be used.

	unsigned int glVersion;

	char *execDir;

	oriWindow *windowListHead;
} _orionState;
extern _orionState _orion;

// ======================================================================================
// ***** 				   		 	HELPER FUNCTIONS								*****
// ======================================================================================

/**
 * @brief Initialise GLFW. This function is called implicitly when the user calls the first Orion-abstracted GLFW function.
 * 
 */
void _orionInitGLFW();

// ======================================================================================
// ***** 				   		 	  ORION ERRORS 									*****
// ======================================================================================

#define ORERR_MULTIPLE_CALLS 	0x001, 	"Illegal call of function that has already been called; this function can not be called more than once.", 		"ORERR_MULTIPLE_CALLS"
#define ORERR_GL_INVALID_VERS 	0x002, 	"Attempted to load invalid OpenGL version.", 																	"ORERR_GL_INVALID_VERS"
#define ORERR_GL_ABOVE_MAX 		0x003, 	"Attempted to load invalid OpenGL version; maximum OpenGL version supported by Orion is 4.6.", 					"ORERR_GL_ABOVE_MAX"
#define ORERR_GL_BELOW_MIN 		0x004, 	"Attempted to load invalid OpenGL version; versions earlier than 1.1 are not supported by Orion.", 				"ORERR_GL_BELOW_MIN"
#define ORERR_NOT_INIT 			0x005, 	"A function was called that requires Orion to be initialised, but Orion has not been initialised!", 			"ORERR_NOT_INIT"
#define ORERR_NULL_RECIEVED 	0x006, 	"A function recieved NULL but it was not able to accept this argument.", 										"ORERR_NULL_RECIEVED"
#define ORERR_GLFW_FAIL 		0x007, 	"GLFW sent an error and Orion failed to recover.",																"ORERR_GLFW_FAIL"
#define ORERR_GL_VERS_TOO_LOW	0x008, 	"A function was called that requires OpenGL to be of a higher version than was specified.", 					"ORERR_GL_VERS_TOO_LOW"
#define ORERR_GL_FAIL			0x009, 	"Failed to load OpenGL.", 																						"ORERR_GL_FAIL"
#define ORERR_MULTIPLE_DEBUG	0x00A, 	"Illegal attempt to enable a debug context after a debug context has already been enabled.", 					"ORERR_MULTIPLE_DEBUG"
#define ORERR_ACCESS_DENIED		0x00B, 	"Couldn't execute a necessary function; access denied.", 														"ORERR_ACCESS_DENIED"
#define ORERR_ACCESS_PHANTOM	0x00C, 	"Attempted to access resource that didn't exist.", 																"ORERR_ACCESS_PHANTOM"
#define ORERR_UNSUPPORTED_TYPE	0x00D, 	"Unsupported type given to function", 																			"ORERR_UNSUPPORTED_TYPE"
#define ORERR_BUFFER_INVALID	0x00E, 	"Invalid type given to buffer", 																				"ORERR_BUFFER_INVALID"

/**
 * @brief Throw an exception to stdout and break the program
 * 
 * @param code the code of the exception
 * @param msg a helpful message for debugging
 * @param label the label of the error
 */
void _orionThrowError(const int code, const char *msg, const char *label);

/**
 * @brief Struct of error callbacks used in Orion functions.
 * 
 */
typedef struct _orionCallbacks {
	void (* glfwErrorCallback)(int, const char *); // GLFW error callback
} _orionCallbacks;
extern _orionCallbacks _oriCallbacks;

// ======================================================================================
// ***** 				   		   ORION DEFAULT CALLBACKS							*****
// ======================================================================================

void _orionDefaultGLFWErrorCallback(int id, const char *msg);

#ifdef __cplusplus
}
#endif

#endif // include guard