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
#include <stdio.h>

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
	unsigned int currentTarget;
	bool dataSet;
	unsigned int dataSize;
} oriBuffer;

typedef struct oriVertexArray {
	oriVertexArray *next;

	unsigned int handle;
} oriVertexArray;


// ======================================================================================
// ***** 				    ORION VERTEX SPECIFICATION FUNCTIONS 					*****
// ======================================================================================

/**
 * @brief Allocate and initialise a new oriVertexArray structure.
 * 
 * @ingroup vertexspec
 */
oriVertexArray *oriCreateVertexArray() {
	_orionAssertVersion(300);

	if (!_orion.initialised) {
		_orionThrowError(ORERR_NOT_INIT);
	}

	oriVertexArray *r = malloc(sizeof(oriVertexArray));
	r->handle = 0;

	// use DSA if possible
	if (_orion.glVersion >= 450) {
		// using glCreateVertexArrays (4.5) means the vao is generated and initialised (glGenVertexArrays only generates it)
		glCreateVertexArrays(1, &r->handle);
	} else {
		glGenVertexArrays(1, &r->handle);
	}

	// add to global linked list
	r->next = _orion.vertexArrayListHead;
	_orion.vertexArrayListHead = r;

	return r;
}

/**
 * @brief Destroy and free memory for the given vertex array.
 * 
 * @param va the vertex array to free.
 * 
 * @ingroup vertexspec
 */
void oriFreeVertexArray(oriVertexArray *va) {
	_orionAssertVersion(300);

	// unlink from global linked list
	oriVertexArray *current = _orion.vertexArrayListHead;
	while (current != va)
		current = current->next;
	_orion.vertexArrayListHead = va->next;

	glDeleteVertexArrays(1, &va->handle);

	free(va);
	va = NULL;
}

/**
 * @brief Bind a vertex array.
 * 
 * @param va the vertex array to bind.
 * 
 * @ingroup vertexspec
 */
void oriBindVertexArray(oriVertexArray *va) {
	_orionAssertVersion(300);

	if (oriCurrentVertexArray() == va->handle) {
		return;
	}
	glBindVertexArray(va->handle);
}

/**
 * @brief Return the OpenGL handle to the given vertex array.
 * 
 * @param va the vertex array to inspect.
 * 
 * @ingroup vertexspec
 */
unsigned int oriGetVertexArrayHandle(oriVertexArray *va) {
	return va->handle;
}

/**
 * @brief Specifies vertex data with the given attribute format.
 * @details @c buffer @b should be a vertex buffer. But it does not necessarily have to be.
 * According to the OpenGL specification:
 *  > [Vertex Buffer Objects] are no different from any other buffer object, and a buffer object used for Transform
 *  > Feedback or asynchronous pixel transfers can be used as source values for vertex arrays.
 * 
 * @warning The buffer still needs to be bound to @c GL_ARRAY_BUFFER when not using direct state access. So
 * if your GL version is below 4.5 and your buffer is not bound to @c GL_ARRAY_BUFFER then you will get a warning in the
 * console and @b the @b function @b will @b exit @b early.
 * 
 * @param va the vertex array object (VAO) to store the vertex data in.
 * @param buffer the buffer to read from.
 * @param index the index of the vertex attribute to be defined.
 * @param size the number of components per vertex attribute.
 * @param type the type of each component, e.g. \c GL_FLOAT or \c GL_INT.
 * @param normalised should the data be normalised
 * @param stride the byte offset between each vertex attribute.
 * @param offset an offset of the first component of the vertex attribute.
 * 
 * @ingroup vertexspec
 */
void oriSpecifyVertexData(oriVertexArray *va, oriBuffer *buffer, 
	const unsigned int index,
	const unsigned int size,
	const unsigned int type,
	const bool normalised,
	const unsigned int stride,
	const unsigned int offset
) {
	if (type == GL_DOUBLE) {
		printf("[Orion : WARN] >> (in oriSpecifyVertexData()) the OpenGL Specification heavily warns against using GL_DOUBLE.\n");
	}
	if (type == GL_UNSIGNED_INT_10F_11F_11F_REV) {
		_orionAssertVersion(440);
		if (size != 3) {
			printf("[Orion : WARN] >> (in oriSpecifyVertexData()) size MUST be 3 when using GL_UNSIGNED_INT_10F_11F_11F_REV.\n");
			return;
		}
	}
	if ((type == GL_INT_2_10_10_10_REV || type == GL_UNSIGNED_INT_2_10_10_10_REV) && size != 4) {
		printf("[Orion : WARN] >> (in oriSpecifyVertexData()) size MUST be 4 when using either GL_INT_2_10_10_10_REV or GL_UNSIGNED_INT_2_10_10_10_REV.\n");
		return;
	}

	_orionAssertVersion(300);

	// 1 = glVertexAttribPointer / glVertexArrayAttribFormat
	// 2 = glVertexAttribIPointer / glVertexArrayAttribIFormat
	// 3 = glVertexAttribLPointer / glVertexArrayAttribLFormat
	unsigned int vertexAttribPointerFuncType = 0;

	// glVertexAttribPointer and its I variant are available
	if (_orion.glVersion < 410) {
		switch (type) {
			case GL_HALF_FLOAT:
			case GL_FLOAT:
			case GL_DOUBLE:
			case GL_FIXED:
			case GL_INT_2_10_10_10_REV:
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				vertexAttribPointerFuncType = 1;
				break;
			default:
				vertexAttribPointerFuncType = 2;
				break;
		}
	}
	// all variants of glVertexAttribPointer are available
	else {
		switch (type) {
			case GL_HALF_FLOAT:
			case GL_FLOAT:
			case GL_FIXED:
			case GL_INT_2_10_10_10_REV:
			case GL_UNSIGNED_INT_2_10_10_10_REV:
			case GL_UNSIGNED_INT_10F_11F_11F_REV:
				vertexAttribPointerFuncType = 1;
				break;
			case GL_DOUBLE:
				vertexAttribPointerFuncType = 3;
				break;
			default:
				vertexAttribPointerFuncType = 2;
				break;
		}
	}

	// Use DSA where possible
	if (_orion.glVersion >= 450) {
		glEnableVertexArrayAttrib(va->handle, index);
		glVertexArrayVertexBuffer(va->handle, index, buffer->handle, 0, stride);
		
		// use appropriate function as decided above to stop data from being converted to floats
		switch (vertexAttribPointerFuncType) {
			case 1:
			default:
				glVertexArrayAttribFormat(va->handle, index, size, type, normalised, offset);
				break;
			case 2:
				glVertexArrayAttribIFormat(va->handle, index, size, type, offset);
				break;
			case 3:
				glVertexArrayAttribLFormat(va->handle, index, size, type, offset);
				break;
		}
		glVertexArrayAttribBinding(va->handle, index, index);

		// I don't understand binding indices at all, so I'm doing what some guy
		// recommended: simply using the attribute index as the binding index.
		// Hopefully some guy is right.

		return;
	}

	// ---
	// if DSA is not possible

	if (buffer->currentTarget != GL_ARRAY_BUFFER) {
		printf("[Orion : WARN] >> (in oriSpecifyVertexData()) when version is below 4.5, the buffer must be bound to GL_ARRAY_BUFFER.\n");
		return;
	}

	unsigned int previousVA = oriCurrentVertexArray();
	unsigned int previousBuffer = oriCurrentBufferAt(GL_ARRAY_BUFFER);

	oriBindVertexArray(va);
	oriBindBuffer(buffer, GL_ARRAY_BUFFER);

	glEnableVertexAttribArray(index);

	// The following code causes the -Wint-to-pointer cast in GCC. It probably causes it in other compilers too, but I only use GCC so I don't know.
	// Unfortunately, the compiler complains about something I can't do anything about: OpenGL's very backwards-compatible specification.
	// (converting from unsigned int to void *)
	// So, the warning is just suppressed here.

#	pragma GCC diagnostic push
#	pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

	// use appropriate function as decided above to stop data from being converted to floats
	switch (vertexAttribPointerFuncType) {
		case 1:
		default:
			glVertexAttribPointer(index, size, type, normalised, stride, (const void *) offset);
			break;
		case 2:
			glVertexAttribIPointer(index, size, type, stride, (const void *) offset);
			break;
		case 3:
			glVertexAttribLPointer(index, size, type, stride, (const void *) offset);
			break;
	}

#	pragma GCC diagnostic pop

	// bind to previous objects
	glBindVertexArray(previousVA);
	glBindBuffer(GL_ARRAY_BUFFER, previousBuffer);
}

// ======================================================================================
// ***** 				   		  ORION BUFFER FUNCTIONS 							*****
// ======================================================================================

/**
 * @brief Allocate and initialise a new oriBuffer structure.
 *  
 * @ingroup buffers
 */
oriBuffer *oriCreateBuffer() {
	if (!_orion.initialised) {
		_orionThrowError(ORERR_NOT_INIT);
	}

	oriBuffer *r = malloc(sizeof(oriBuffer));
	r->handle = 0;
	r->dataSet = false;
	r->dataSize = 0;
	r->currentTarget = 0;

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
 * @param target the OpenGL buffer target to bind the buffer to.
 * 
 * @ingroup buffers
 */
void oriBindBuffer(oriBuffer *buffer, unsigned int target) {
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

	if (oriCurrentBufferAt(target) == buffer->handle) {
		return;
	}
	glBindBuffer(target, buffer->handle);
	buffer->currentTarget = target;
}

/**
 * @brief Return the OpenGL handle to the given buffer struct.
 * 
 * @param buffer the buffer to inspect.
 * 
 * @ingroup buffers
 */
unsigned int oriGetBufferHandle(oriBuffer *buffer) {
	return buffer->handle;
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

	// for the sake of supporting non-DSA, the buffer will be temporarily bound to GL_ARRAY_BUFFER during this function's lifespan.

	// if DSA is not possible then the buffer needs to be bound before anything else.
	unsigned int boundCache = 0;
	if (!dsaEnabled) {
		// boundCache will be bound when the function is done
		boundCache = oriCurrentBufferAt(GL_ARRAY_BUFFER);
		oriBindBuffer(buffer, GL_ARRAY_BUFFER);
	}

	// reallocate space for the data if the data hasn't been set or if the size has changed
	if (!buffer->dataSet || buffer->dataSize != size) {
		buffer->dataSize = size;
		
		if (dsaEnabled) {
			glNamedBufferData(buffer->handle, size, data, usage);
		} else {
			glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		}

		buffer->dataSet = true;

		// unbind to last buffer if necessary
		if (!dsaEnabled) {
			glBindBuffer(GL_ARRAY_BUFFER, boundCache);
		}

		// return early
		return;
	}

	// if data has already been set at least once and the size is still the same then just change the data store
	if (dsaEnabled) {
		glNamedBufferSubData(buffer->handle, 0, size, data);
	} else {
		glBufferSubData(GL_ARRAY_BUFFER , 0, size, data);
	}

	// unbind to last buffer if necessary
	if (!dsaEnabled) {
		glBindBuffer(GL_ARRAY_BUFFER, boundCache);
	}
}