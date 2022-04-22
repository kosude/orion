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

#include "oriongl.h"
#include "orionwin.h"

#include "internal.h"
#include "platform.h" // cmake-generated platform info
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <libgen.h>
#include <stdio.h>

// global state structure
_orionState _orion = { NULL };

/**
 * @brief Initialise GLFW. This function is called implicitly when the user calls the first Orion-abstracted GLFW function.
 * 
 */
void _orionInitGLFW() {
	// initialise GLFW
	glfwSetErrorCallback(_oriCallbacks.glfwErrorCallback);
	glfwInit();

	_orion.glfwInitialised = true;
}

/**
 * @brief throw an exception to stdout and break the program
 * 
 * @param code the code of the exception
 * @param msg a helpful message for debugging
 * @param label the label of the error
 */
void _orionThrowError(const int code, const char *msg, const char *label) {
	printf("[Orion : FATAL!] >> Error code 0x%03hhX (%s) : %s\n", code, label, msg);
	__debugbreak;

	if (_orion.initialised) {
		oriTerminate();
	}
	exit(-1);
}

/**
 * @brief If the initialised OpenGL version (that given to oriInitialise()) is below the given minimum, throw an exception.
 * 
 */
void _orionAssertVersion(unsigned int minimum) {
	if (!_orion.glLoaded) {
		_orionThrowError(ORERR_GL_NOT_LOADED);
	}
	if (_orion.glVersion < minimum) {
		printf("[Orion : VERSERR] >> Loaded version %d is not high enough to meet minimum of %d.\n", _orion.glVersion, minimum);
		_orionThrowError(ORERR_GL_OLD_VERS);
	}
}

// ======================================================================================
// ***** 				   ORION PUBLIC INITIALISATION FUNCTIONS 					*****
// ======================================================================================

/**
 * @brief Initialise the global (internal) Orion state.
 * 
 * @param version the version of OpenGL that is being used.
 * 
 * @ingroup meta
 */
void oriInitialise(const unsigned int version) {
	if (_orion.initialised) {
		_orionThrowError(ORERR_MULTIPLE_CALLS);
	}

	// validate arguments
	if (version % 10 && version != 121) {
		// version must be a multiple of 10 (unless it is 1.2.1, the only exception)
		_orionThrowError(ORERR_GL_INVALID_VERS);
	}
	if (version == 0) {
		// can't accept NULL/0 as an argument
		_orionThrowError(ORERR_NULL_RECIEVED);
	}
	if (version > 460) {
		// 4.6 is currently the latest OpenGL version - any higher is invalid
		_orionThrowError(ORERR_GL_ABOVE_MAX);
	}
	if (version < 110) {
		// dont accept any earlier versions than 1.1
		_orionThrowError(ORERR_GL_BELOW_MIN);
	}
	if (version > 150 && version < 200) {
		// invalid version
		_orionThrowError(ORERR_GL_INVALID_VERS);
	}
	if (version > 210 && version < 300) {
		// invalid version
		_orionThrowError(ORERR_GL_INVALID_VERS);
	}
	if (version > 330 && version < 400) {
		// invalid version
		_orionThrowError(ORERR_GL_INVALID_VERS);
	}

	_orion.glVersion = version;

	// get path of executable
	// TODO: Unix-only

	// exe symlink MUST be at /proc/self/exe
	// this is a pretty annoying compatibility issue, and should be fixed later
	// (only Unix-like systems that use /proc/self/exe, like Linux, are supported)
	if (access("/proc/self/exe", 0) != 0) {
		_orionThrowError(ORERR_ACCESS_DENIED);
	}

	// read symlink
	char buf[256];
	realpath("/proc/self/exe", buf);

	// convert to directory name
	char p[256];
	strncpy(p, dirname(buf), 256);

	// store in state object
	_orion.execDir = malloc(256 * sizeof(char));
	strncpy(_orion.execDir, p, 256);

	// change working directory to wherever the executable is
	if (chdir(_orion.execDir) != 0) {
		_orionThrowError(ORERR_ACCESS_PHANTOM);
	}

	_orion.initialised = true;
}

/**
 * @brief Terminate the Orion library. All Orion GL objects that were allocated will be freed.
 * 
 * @warning If you are using a third-party library to handle your OpenGL context rather than Orionwin,
 * make sure that you call this function @b before the OpenGL context gets terminated. For example (if you're using GLFW),
 * call @c glfwTerminate() after this function.
 * 
 * @ingroup meta
 */
void oriTerminate() {
	// destroy all shader objects
	while (_orion.shaderListHead) {
		oriFreeShader(_orion.shaderListHead);
	}
	// destroy all buffer objects
	while (_orion.bufferListHead) {
		oriFreeBuffer(_orion.bufferListHead);
	}
	// destroy all vertex array objects
	while (_orion.vertexArrayListHead) {
		oriFreeVertexArray(_orion.vertexArrayListHead);
	}

	// destroy all window objects
	while (_orion.windowListHead) {
		oriFreeWindow(_orion.windowListHead);
	}
	// terminate GLFW
	if (_orion.glfwInitialised) {
		glfwTerminate();
	}

	// free malloc'd state members
	free(_orion.execDir);

	// clear state (reset to nil)
	memset(&_orion, 0, sizeof(_orion));
}

/**
 * @brief Load OpenGL functionality for the given process. This will @b automatically @b be @b done when you create windows with @c orionwin.h.
 * 
 * @param loadproc the process to refer to. If you're using default GLFW abstractions, use @c glfwGetProcAddress.
 * 
 * @ingroup meta
 */
void oriLoadGL(void *(* loadproc)(const char *)) {
	// load OpenGL with Glad
	if (!gladLoadGLLoader(loadproc)) {
		_orionThrowError(ORERR_GL_FAIL);
	}

	_orion.glLoaded = true;
}

/**
 * @brief Enables OpenGL debug context (only available in OpenGL versions 4.3 and above!) - OpenGL errors will be printed to \e stdout.
 * \warning Attempting to run this function on OpenGL functions below version 4.3 will result in an \b exception.
 * 
 * @param source the source to filter the errors to.
 * @param type the type to filter the errors to.
 * @param severity the severity to filter the errors to.
 * @param enabled if true, messages that meet the filter criteria are shown. if false, they are hidden.
 * @param suppressed an array of error IDs to suppress.
 * @param count the size of the array suppressed.
 * 
 * @ingroup meta
 */
void oriEnableDebugContext(const unsigned int source, const unsigned int type, const unsigned int severity, const bool enabled, const unsigned int *suppressed, const unsigned int count) {
	_orionAssertVersion(430);

	// can't be called more than once
	if (_orion.debug) {
		_orionThrowError(ORERR_MULTIPLE_CALLS);
	}

	// assert initialisation
	if (!_orion.initialised) {
		_orionThrowError(ORERR_NOT_INIT);
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(_oriCallbacks.debugMessageCallback, NULL);

	glDebugMessageControl(source, type, severity, count, suppressed, enabled);

	_orion.debug = true;
}