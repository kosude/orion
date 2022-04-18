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

#include "init.h"

#include "internal/internal.h"
#include "platform.h" // cmake-generatd platform info
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#	include <Windows.h> // for getting the executable directory on Windows
#elif defined(UNIX)
#	include <unistd.h>
#	include <libgen.h>
#endif

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
	// TODO: doesn't yet support macos
	#ifdef UNIX
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
	#elif defined(WIN32)
		// use stupid microsoft function
		GetModuleFileNameA(NULL, _orion.execDir, 256);
	#endif

	// change working directory to wherever the executable is
	// NOTE! this should check if the cwd is already in the right place to save performance...
	//		 ...I have tried that, but C strings decided to be annoying and I got tired of the resulting segfaults.
	if (chdir(_orion.execDir) != 0) {
		// error, give up
		_orionThrowError(ORERR_ACCESS_PHANTOM);
	} else {
		// no error, don't give up :)
	}
}