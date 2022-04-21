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

#include <stdbool.h>

#include <glad/4.6/glad.h>
#include <glad/4.6/orionglad/orionglad.h>

#include "shaderpresets.h" // preset shaders

// ======================================================================================
// ***** 				   		    ORION PUBLIC MACROS 							*****
// ======================================================================================

/**
 * @brief The location of the @b vertex @b position vertex attribute in preset Orion shaders.
 * 
 * @ingroup shaders
 */
#define ORION_VERTEX_POS 0
/**
 * @brief The location of the @b texture @b coordinate vertex attribute in preset Orion shaders.
 * 
 * @ingroup shaders
 */
#define ORION_TEXTURE_COORD 1
/**
 * @brief The location of the @b vertex @b colour vertex attribute in preset Orion shaders.
 * 
 * @ingroup shaders
 */
#define ORION_VERTEX_COLOUR 2

// ======================================================================================
// ***** 				   		 ORION PUBLIC STRUCTURES 							*****
// ======================================================================================

/**
 * @brief An OpenGL shader program object.
 * 
 * @ingroup shaders
 */
typedef struct oriShader oriShader;

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
void oriInitialise(const unsigned int version);

/**
 * @brief Terminate the Orion library. All Orion objects that were allocated (e.g. shaders) will be freed.
 * 
 * @ingroup meta
 */
void oriTerminate();

/**
 * @brief Load OpenGL functionality for the given process. This will @b automatically @b be @b done when you create windows with @c orionwin.h.
 * 
 * @param loadproc the process to refer to. If you're using default GLFW abstractions, use @c glfwGetProcAddress.
 * 
 * @ingroup meta
 */
void oriLoadGL(void *(* loadproc)(const char *));

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
void oriEnableDebugContext(const unsigned int source, const unsigned int type, const unsigned int severity, const bool enabled, const unsigned int *suppressed, const unsigned int count);

// ======================================================================================
// ***** 				   		  ORION SHADER FUNCTIONS 							*****
// ======================================================================================

/**
 * @brief Allocate and initialise a new oriShader structure.
 * 
 * @ingroup shaders
 */
oriShader *oriCreateShader();

/**
 * @brief Destroy and free memory for the given shader.
 * @param shader the shader to free
 * 
 * @note This will automatically be done in a call to oriTerminate().
 * 
 * @ingroup shaders
 */
void oriFreeShader(oriShader *shader);

/**
 * @brief Compile and error-check the given GLSL source code.
 * 
 * @param type the type of shader (e.g. @c GL_FRAGMENT_SHADER)
 * @param src the source code to compile
 * @return 0 if the shader did not successfully compile.
 * 
 * @ingroup shaders
 */
unsigned int oriCompileShader(const unsigned int type, const char *src);

/**
 * @brief Parse a shader file and return it as a single string.
 * 
 * @param path the path to the shader, @b relative @b to @b the @b executable!
 * 
 * @ingroup shaders
 */
const char *oriParseShader(const char *path);

/**
 * @brief Add GLSL source to the given shader.
 * 
 * @param shader the shader to modify
 * @param type the type of source code (e.g. @c GL_VERTEX_SHADER)
 * @param src the source code to add, as a string
 * 
 * @ingroup shaders
 */
void oriAddShaderSource(oriShader *shader, const unsigned int type, const char *src);

/**
 * @brief Bind a shader struct.
 * 
 * @param shader the shader to bind.
 * 
 * @ingroup shaders
 */
void oriBindShader(oriShader *shader);

/**
 * @brief Returns the bound state of the given shader.
 * 
 * @param shader the shader to inspect.
 * @return true if the shader program is being used.
 * @return false if the shader program is not being used.
 * 
 * @ingroup shaders
 */
bool oriShaderIsBound(oriShader *shader);

/**
 * @brief Get the location of a GLSL uniform by its name
 * @details If the given uniform has not been cached in the oriShader struct, it will be retrieved with glGetUniformLocation() and @e then cached for later use.
 * 
 * @param shader the shader to inspect.
 * @param name the name of the uniform
 * 
 * @ingroup shaders
 */
int oriShaderGetUniformLocation(oriShader *shader, const char *name);

// ----------------
// oriSetShaderUniform :: scalars

/** @ingroup shaders */ void oriSetUniform1i(oriShader *shader, const char *name, const int val);
/** @ingroup shaders */ void oriSetUniform1f(oriShader *shader, const char *name, const float val);
/** @ingroup shaders */ void oriSetUniform1ui(oriShader *shader, const char *name, const unsigned int val);

// ----------------
// oriSetShaderUniform :: vectors

/** @ingroup shaders */ void oriSetUniform2i(oriShader *shader, const char *name, const int x, const int y);
/** @ingroup shaders */ void oriSetUniform2f(oriShader *shader, const char *name, const float x, const float y);
/** @ingroup shaders */ void oriSetUniform2ui(oriShader *shader, const char *name, const unsigned int x, const unsigned int y);

/** @ingroup shaders */ void oriSetUniform3i(oriShader *shader, const char *name, const int x, const int y, const int z);
/** @ingroup shaders */ void oriSetUniform3f(oriShader *shader, const char *name, const float x, const float y, const float z);
/** @ingroup shaders */ void oriSetUniform3ui(oriShader *shader, const char *name, const unsigned int x, const unsigned int y, const unsigned int z);

/** @ingroup shaders */ void oriSetUniform4i(oriShader *shader, const char *name, const int x, const int y, const int z, const int w);
/** @ingroup shaders */ void oriSetUniform4f(oriShader *shader, const char *name, const float x, const float y, const float z, const float w);
/** @ingroup shaders */ void oriSetUniform4ui(oriShader *shader, const char *name, const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int w);

// ----------------
// oriSetShaderUniform :: matrices

/** @ingroup shaders */ void oriSetUniformMat2x2f(oriShader *shader, const char *name, const bool transpose, const float *mat);
/** @ingroup shaders */ void oriSetUniformMat2x3f(oriShader *shader, const char *name, const bool transpose, const float *mat);
/** @ingroup shaders */ void oriSetUniformMat2x4f(oriShader *shader, const char *name, const bool transpose, const float *mat);
/** @ingroup shaders */ void oriSetUniformMat3x2f(oriShader *shader, const char *name, const bool transpose, const float *mat);
/** @ingroup shaders */ void oriSetUniformMat3x3f(oriShader *shader, const char *name, const bool transpose, const float *mat);
/** @ingroup shaders */ void oriSetUniformMat3x4f(oriShader *shader, const char *name, const bool transpose, const float *mat);
/** @ingroup shaders */ void oriSetUniformMat4x2f(oriShader *shader, const char *name, const bool transpose, const float *mat);
/** @ingroup shaders */ void oriSetUniformMat4x3f(oriShader *shader, const char *name, const bool transpose, const float *mat);
/** @ingroup shaders */ void oriSetUniformMat4x4f(oriShader *shader, const char *name, const bool transpose, const float *mat);

#ifdef __cplusplus
}
#endif

#endif // include guard