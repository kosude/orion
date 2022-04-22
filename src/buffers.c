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
 * @brief An OpenGL buffer object.
 * 
 * @ingroup buffers
 */
typedef struct oriBuffer {
	oriBuffer *next;

	unsigned int handle;
	unsigned int target;
	bool dataSet;
	unsigned int dataSize;
} oriBuffer;

// ======================================================================================
// ***** 				   		  ORION BUFFER FUNCTIONS 							*****
// ======================================================================================

/**
 * @brief Allocate and initialise a new oriBuffer structure.
 * 
 * @param target the buffer target (e.g. GL_ARRAY_BUFFER).
 * 
 * @ingroup buffers
 */
oriBuffer *oriCreateBuffer(unsigned int target) {
	// assert the correct version
	// see https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml
	switch (target) {
		case GL_COPY_READ_BUFFER:
		case GL_UNIFORM_BUFFER:
		case GL_TEXTURE_BUFFER:
			_orionAssertVersion(310);
			break;
		case GL_ATOMIC_COUNTER_BUFFER:
			_orionAssertVersion(420);
		case GL_DISPATCH_INDIRECT_BUFFER:
		case GL_SHADER_STORAGE_BUFFER:
			_orionAssertVersion(430);
		case GL_QUERY_BUFFER:
			_orionAssertVersion(440);
		default:
			_orionAssertVersion(200);
	}

	if (!_orion.initialised) {
		_orionThrowError(ORERR_NOT_INIT);
	}

	oriBuffer *r = malloc(sizeof(oriBuffer));
	r->handle = 0;
	r->dataSet = false;
	r->dataSize = 0;
	r->target = target;

	// use DSA if possible
	if (_orion.glVersion >= 450) {
		// using glCreateBuffers (4.5) means the buffer object is generated and initialised (glGenBuffers only generates it)
		glCreateBuffers(1, &r->handle);
	} else {
		glGenBuffers(1, &r->handle);
	}

	r->next = _orion.bufferListHead;
	_orion.bufferListHead = r;

	return r;
}

/**
 * @brief Destroy and free memory for the given buffer.
 * 
 * @param buffer the buffer to free
 * 
 * @ingroup buffers
 */
void oriFreeBuffer(oriBuffer *buffer) {
	_orionAssertVersion(200);

	// unlink from global linked list
	oriBuffer *current = _orion.bufferListHead;
	while (current != buffer)
		current = current->next;
	_orion.bufferListHead = buffer->next;

	glDeleteBuffers(1, &buffer->handle);

	// free buffer
	free(buffer);
	buffer = NULL; 
}

/**
 * @brief Bind the given buffer.
 * 
 * @param buffer the buffer to bind.
 * 
 * @ingroup buffers
 */
void oriBindBuffer(oriBuffer *buffer) {
	_orionAssertVersion(200);

	if (oriCurrentBufferAt(buffer->target) == buffer->handle) {
		return;
	}
	glBindBuffer(buffer->target, buffer->handle);
}

/**
 * @brief Set data in a given oriBuffer structure.
 * 
 * @param buffer the buffer to copy data into.
 * @param data the data to copy in @c buffer.
 * @param size the size of the given data.
 * @param usage the usage of the buffer.
 * 
 * @ingroup buffers
 */
void oriSetBufferData(oriBuffer *buffer, const void *data, const unsigned int size, const unsigned int usage) {
	_orionAssertVersion(200);

	bool dsaEnabled = _orion.glVersion >= 450;

	// if DSA is not possible then the buffer needs to be bound before anything else.
	unsigned int boundCache = 0;
	if (!dsaEnabled) {
		// boundCache will be bound when the function is done
		boundCache = oriCurrentBufferAt(buffer->target);
		oriBindBuffer(buffer);
	}

	// reallocate space for the data if the data hasn't been set or if the size has changed
	if (!buffer->dataSet || buffer->dataSize != size) {
		buffer->dataSize = size;
		
		if (dsaEnabled) {
			glNamedBufferData(buffer->handle, size, data, usage);
		} else {
			glBufferData(buffer->target, size, data, usage);
		}

		buffer->dataSet = true;

		// unbind to last buffer if necessary
		if (!dsaEnabled) {
			glBindBuffer(buffer->target, boundCache);
		}

		// return early
		return;
	}

	// if data has already been set at least once and the size is still the same then just change the data store
	if (dsaEnabled) {
		glNamedBufferSubData(buffer->handle, 0, size, data);
	} else {
		glBufferSubData(buffer->target , 0, size, data);
	}

	// unbind to last buffer if necessary
	if (!dsaEnabled) {
		glBindBuffer(buffer->target, boundCache);
	}
}