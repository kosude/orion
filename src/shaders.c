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
#include "oriongl.h"

#include <stdlib.h>

// ======================================================================================
// ***** 				   		 ORION PUBLIC STRUCTURES 							*****
// ======================================================================================

/**
 * @brief An OpenGL shader program object.
 * 
 * @ingroup shaders
 */
typedef struct oriShader {
	oriShader *next;

	unsigned int handle;
	const char *src;
} oriShader;

// ======================================================================================
// ***** 				   		  ORION SHADER FUNCTIONS 							*****
// ======================================================================================

/**
 * @brief Allocate and initialise a new oriShader structure.
 * 
 */
oriShader *oriCreateShader() {
	if (!_orion.initialised) {
		_orionThrowError(ORERR_NOT_INIT);
	}

	oriShader *r = malloc(sizeof(oriShader));

	r->handle = glCreateProgram();

	// link to global linked list (add to the start)
	r->next = _orion.shaderListHead;
	_orion.shaderListHead = r;
}

/**
 * @brief Destroy and free memory for the given shader.
 * 
 * @ingroup shaders
 */
void oriFreeShader(oriShader *shader) {
	// unlink from global linked list
	oriShader **prev = &_orion.shaderListHead;
	while (*prev != shader)
		*prev = (*prev)->next;
	*prev = shader->next;

	// free shader
	free(shader);
	shader = NULL;
}