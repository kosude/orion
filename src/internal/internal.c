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

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <stdio.h>

#ifdef SIGTRAP
#	define __debugbreak raise(SIGTRAP)
#else
#	define __debugbreak raise(SIGABRT)
#endif

// global state structure
_orionState _orion = { 0 };

/**
 * @brief throw an exception to stdout and break the program
 * 
 * @param code the code of the exception
 * @param msg a helpful message for debugging
 * @param label the label of the error
 */
void _orionThrowError(const int code, const char *msg, const char *label) {
	printf(	"\n(!) -----------------\n"
			"  A fatal Orion GL exception has been encountered, and the program has been halted.\n"
			"  Description: \"%s\"\n"
			"  Code: 0x%03hhX (%s)\n"
			"(!) -----------------\n",
		msg, code, label
	);
	__debugbreak;
	exit(-1);
}