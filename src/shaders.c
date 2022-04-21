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
// ***** 				   		 ORION INTERNAL DATA TYPES 							*****
// ======================================================================================

typedef struct _oriUniform {
	struct _oriUniform *next;

	int location;
	const char *name;
} _oriUniform;

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

	// linked list to cache uniform locations in the shader program
	_oriUniform *uniformListHead;
} oriShader;

// ======================================================================================
// ***** 				   		  ORION SHADER FUNCTIONS 							*****
// ======================================================================================

/**
 * @brief Allocate and initialise a new oriShader structure.
 * 
 */
oriShader *oriCreateShader() {
	_orionAssertVersion(200);

	if (!_orion.initialised) {
		_orionThrowError(ORERR_NOT_INIT);
	}

	oriShader *r = malloc(sizeof(oriShader));

	r->uniformListHead = NULL;
	r->src = NULL;

	r->handle = glCreateProgram();

	// link to global linked list (add to the start)
	r->next = _orion.shaderListHead;
	_orion.shaderListHead = r;

	return r;
}

/**
 * @brief Destroy and free memory for the given shader.
 * 
 * @ingroup shaders
 */
void oriFreeShader(oriShader *shader) {
	_orionAssertVersion(200);

	// free the shader uniforms linked list
	_oriUniform *u_next = NULL;
	
	while (shader->uniformListHead) {
		u_next = shader->uniformListHead->next;

		free(shader->uniformListHead);

		shader->uniformListHead = u_next;
	}

	// unlink from global linked list
	oriShader **current = &_orion.shaderListHead;
	while (*current != shader)
		*current = (*current)->next;
	_orion.shaderListHead = shader->next;

	// opengl delete program
	glDeleteProgram(shader->handle);

	// free shader
	free(shader);
	shader = NULL;
}

/**
 * @brief Compile and error-check the given GLSL source code.
 * 
 * @param type the type of shader (e.g. @c GL_FRAGMENT_SHADER)
 * @param src the source code to compile
 * @return 0 if the shader did not successfully compile.
 * 
 * @ingroup shaders
 */
unsigned int oriCompileShader(const unsigned int type, const char *src) {
	_orionAssertVersion(200);

	unsigned int id = glCreateShader(type);

	// input shader code from src
	glShaderSource(id, 1, &src, NULL);

	// compile
	glCompileShader(id);
	int status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	// if compilation failed
	if (!status) {
		// get length of error message
		int len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

		// get error
		char *e = malloc(len * sizeof(char));
		glGetShaderInfoLog(id, len, &len, e);

		// log error to stdout
		printf("[Orion : SHADER] >> %s\n", e);

		free(e);

		glDeleteShader(id);
		return 0;
	}
	
	// if compilation succeeded then return the new compiled shader ID
	return id;
}

/**
 * @brief Parse a shader file and return it as a single string.
 * 
 * @param path the path to the shader, @b relative @b to @b the @b executable!
 * 
 * @ingroup shaders
 */
const char *oriParseShader(const char *path) {
	// ----
	// method courtesy of Joe Cool on stackoverflow
	// ----

	char *buffer;
	unsigned int length;
	FILE *f = fopen(path, "rb");
	unsigned readlength;

	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);

		// 1 GiB is a reasonable limit
		if (length > 1073741824) {
			_orionThrowError(ORERR_FILE_TOO_LARGE);
		}

		buffer = malloc((length + 1) * sizeof(char)); // +1 for null terminator.

		if (length) {
			readlength = fread(buffer, 1, length, f);

			if (length != readlength) {
				free(buffer);
				_orionThrowError(ORERR_FILE_READ_ERROR);
			}
		}

		fclose(f);

		buffer[length] = '\0';
	} else {
		_orionThrowError(ORERR_ACCESS_PHANTOM);
	}

	return buffer;
}

/**
 * @brief Add GLSL source to the given shader.
 * 
 * @param shader the shader to modify
 * @param type the type of source code (e.g. @c GL_VERTEX_SHADER)
 * @param src the source code to add, as a string
 * 
 * @ingroup shader
 */
void oriAddShaderSource(oriShader *shader, const unsigned int type, const char *src) {
	_orionAssertVersion(200);

	// compile + attach
	unsigned int id = oriCompileShader(type, src);
	glAttachShader(shader->handle, id);

	// link and validate program
	glLinkProgram(shader->handle);
	glValidateProgram(shader->handle);

	// free shader
	glDeleteShader(id);
}

/**
 * @brief Bind a shader struct.
 * 
 * @param shader the shader to bind.
 */
void oriBindShader(oriShader *shader) {
	_orionAssertVersion(200);

	if (oriShaderIsBound(shader)) {
		return;
	}
	glUseProgram(shader->handle);
}

/**
 * @brief Returns the bound state of the given shader.
 * 
 * @param shader the shader to inspect.
 * @return true if the shader program is being used.
 * @return false if the shader program is not being used.
 */
bool oriShaderIsBound(oriShader *shader)  {
	return oriCurrentShaderProgram() == shader->handle;
}

/**
 * @brief Get the location of a GLSL uniform by its name
 * @details If the given uniform has not been cached in the oriShader struct, it will be retrieved with glGetUniformLocation() and @e then cached for later use.
 * 
 * @param shader the shader to inspect.
 * @param name the name of the uniform
 */
int oriShaderGetUniformLocation(oriShader *shader, const char *name) {
	_oriUniform *current = shader->uniformListHead;
	
	while (current && current->name != name) {
		current = current->next;
	}

	// the uniform with the given name has been found in the uniforms cache
	if (current) {
		return current->location;
	}
	// the given name has not been found in the uniforms cache
	else {
		_orionAssertVersion(200);

		int r = glGetUniformLocation(shader->handle, name);

		if (r < 0) {
			// OpenGL didn't get the location
			printf("[Orion : WARN] >> glGetUniformLocation() with uniform name %s failed!", name);
			return 0;
		}

		// store r in cache (link a new uniform node to the uniforms linked list)
		_oriUniform *new = malloc(sizeof(_oriUniform));
		new->location = r;
		new->name = name;
		new->next = shader->uniformListHead;
		shader->uniformListHead = new;

		// return r
		return r;
	}
}

#define __setUniformHelper(glfunc, version) {\
	_orionAssertVersion(version);\
	unsigned int _boundCache = oriCurrentShaderProgram();\
	oriBindShader(shader);\
	glfunc;\
	glUseProgram(_boundCache);\
}

/** @ingroup shaders */ void oriSetShaderUniformi(oriShader *shader, const char *name, const int val) {
	__setUniformHelper(glUniform1i(oriShaderGetUniformLocation(shader, name), val), 200);
}