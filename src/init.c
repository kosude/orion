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

#include "internal.h"
#include "platform.h" // cmake-generated platform info
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <libgen.h>
#include <stdio.h>

// global state structure
_orionState _orion = { 0 };

// global callbacks
_orionCallbacks _oriCallbacks = {
	_orionDefaultGLFWErrorCallback
};

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
	exit(-1);
}

// ======================================================================================
// ***** 				   ORION PUBLIC INITIALISATION FUNCTIONS 					*****
// ======================================================================================

/**
 * @brief initialise the global (internal) Orion state.
 * 
 * @param version the version of OpenGL that is to be used.
 * @param profile the OpenGL profile to use.
 * 
 * @ingroup meta
 */
void oriInitialise(const unsigned int version, const unsigned int profile) {
	if (_orion.initialised) {
		_orionThrowError(ORERR_MULTIPLE_CALLS);
	}

	// validate arguments
	if (version % 10 && version != 121) {
		// version must be a multiple of 10 (unless it is 1.2.1, the only exception)
		_orionThrowError(ORERR_GL_INVALID_VERS);
	}
	if (version == 0 || profile == 0) {
		// can't accept NULL/0 for either argument
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

	_orion.initialised = true;
	_orion.glVersion = version;
	_orion.glProfile = profile;

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
}

/**
 * @brief Terminate the Orion library. All Orion GL objects that were allocated will be freed.
 * 
 */
void oriTerminate() {
	// free all windows
	
}