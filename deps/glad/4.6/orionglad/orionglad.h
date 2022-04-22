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
 * @file orionglad.h
 * @brief Orion abstractions for some OpenGL functions
 * @author Jack Bennett
 * 
 */

#pragma once
#ifndef __ORIONGLAD_H
#define __ORIONGLAD_H

#include "../glad.h"

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================================================
// ***** 				     	 NEW / ADDED FUNCTIONALITY 							*****
// ======================================================================================

// ======================================================================================
// ***** 				   	  ADDED FUNCTIONALITY :: BUFFERS 						*****
// ======================================================================================

/**
 * @brief the current GL object that is bound to \c target
 * @details It is recommended to refer to this as opposed to calling glGetx functions for better performance.
 * 
 * @param target the target to query
 * 
 * @ingroup orionglad
 */
const GLuint orion_glCurrentBufferAt(GLenum target);

/**
 * @brief get the target of the given buffer
 * @details E.g, if buffer \c buffer is bound at \c GL_ARRAY_BUFFER, then \c GL_ARRAY_BUFFER will be returned.
 * 
 * @param buffer specifies the name of the buffer to query
 * 
 * @return 0 if \c buffer is not bound.
 * 
 * @ingroup orionglad
 */
const GLenum orion_glGetBufferTarget(GLuint buffer);

// ======================================================================================
// ***** 				      ADDED FUNCTIONALITY :: TEXTURES 						*****
// ======================================================================================

/**
 * @brief the current GL texture that is bound to \c target
 * @details It is recommended to refer to this as opposed to calling glGetx functions for better performance.
 * 
 * @param target the target to query
 * 
 * @ingroup orionglad
 */
const GLuint orion_glCurrentTextureAt(GLenum target);

/**
 * @brief get the target of the given texture
 * @details E.g, if texture \c tex is bound at \c GL_TEXTURE_2D, then \c GL_TEXTURE_2D will be returned.
 * 
 * @param tex specifies the name of the texture to query
 * 
 * @return 0 if \c tex is not bound.
 * 
 * @ingroup orionglad
 */
const GLenum orion_glGetTextureTarget(GLuint tex);

// ======================================================================================
// ***** 				   ADDED FUNCTIONALITY :: VERTEX ARRAYS 					*****
// ======================================================================================

/**
 * @brief the currently-bound GL vertex array object
 * @details It is recommended to refer to this as opposed to calling glGetx functions for better performance.
 * 
 * @ingroup orionglad
 */
const GLuint orion_glCurrentVertexArray();

// ======================================================================================
// ***** 				      ADDED FUNCTIONALITY :: SHADERS 						*****
// ======================================================================================

/**
 * @brief the currently-used GL shader program object
 * @details It is recommended to refer to this as opposed to calling glGetx functions for better performance.
 * 
 * @ingroup orionglad
 */
const GLuint orion_glCurrentShaderProgram();

// ======================================================================================
// ***** 				     OVERRIDES OF EXISTING GL FUNCTIONS 					*****
// ======================================================================================

/** @cond */ // don't document this part

// ======================================================================================
// ***** 				      		OVERRIDES :: BUFFERS 							*****
// ======================================================================================

/**
 * @brief binds a GL buffer object of name \c buffer to \c target
 * 
 * @param target specifies the target to which the buffer point is bound
 * @param buffer specifies the name of a buffer object
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glBindBuffer(GLenum target, GLuint buffer);

/**
 * @brief deletes named buffer objects
 * 
 * @param n the number of buffer objects to be deleted
 * @param buffers specifies an array of buffer objects to be deleted
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glDeleteBuffers(GLsizei n, const GLuint *buffers);

// ======================================================================================
// ***** 				      		OVERRIDES :: TEXTURES 							*****
// ======================================================================================

/**
 * @brief bind a named texture to a texturing target
 * 
 * @param target specifies the target to which the texture is bound
 * @param texture specifies the name of a texture.
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glBindTexture(GLenum target, GLuint texture);

/**
 * @brief deletes named textures
 * 
 * @param n the number of texturess to be deleted
 * @param textures specifies an array of textures to be deleted
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glDeleteTextures(GLsizei n, const GLuint *textures);

// ======================================================================================
// ***** 					     OVERRIDES :: VERTEX ARRAYS 						*****
// ======================================================================================

/**
 * @brief binds a GL vertex array object of name \c array
 * 
 * @param array the name of the vertex array
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glBindVertexArray(GLuint array);

/**
 * @brief deletes named vertex array objects
 * 
 * @param n the number of vertex array objects to be deleted
 * @param arrays specifies an array of vertex array objects to be deleted
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glDeleteVertexArrays(GLsizei n, const GLuint *arrays);

// ======================================================================================
// ***** 					     	OVERRIDES :: SHADERS 							*****
// ======================================================================================

/**
 * @brief installs a program object as part of current rendering state
 * 
 * @param program specifies the handle of the program object whose executables are to be used as part of current rendering state.
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glUseProgram(GLuint program);

/**
 * @brief deletes a GL shader program object
 * 
 * @param program specifies the program object to be deleted.
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glDeleteProgram(GLuint program);

/** @endcond */

// ======================================================================================
// ***** 		 		DEFINE MACROS FOR ADDED ORIONGLAD FUNCTIONS 				*****
// ======================================================================================

#ifndef ORIONGLAD_IMPLEMENTATION
#	define oriGetBufferTarget orion_glGetBufferTarget
#	define oriGetTextureTarget orion_glGetTextureTarget
#	define oriCurrentVertexArray orion_glCurrentVertexArray
#	define oriCurrentShaderProgram orion_glCurrentShaderProgram
#	define oriCurrentBufferAt orion_glCurrentBufferAt
#	define oriCurrentTextureAt orion_glCurrentTextureAt
#endif

// ======================================================================================
// ***** 		  REPLACE GLAD FUNCTIONS WITH ABSTRACTED (ORIONGLAD) ONES 			*****
// ======================================================================================

/** @cond */

#ifndef ORIONGLAD_IMPLEMENTATION
#	undef glBindBuffer
#	define glBindBuffer orion_gladoverride_glBindBuffer

#	undef glDeleteBuffers
#	define glDeleteBuffers orion_gladoverride_glDeleteBuffers

#	undef glBindVertexArray
#	define glBindVertexArray orion_gladoverride_glBindVertexArray

#	undef glDeleteVertexArrays
#	define glDeleteVertexArrays orion_gladoverride_glDeleteVertexArrays

#	undef glUseProgram
#	define glUseProgram orion_gladoverride_glUseProgram

#	undef glDeleteProgram
#	define glDeleteProgram orion_gladoverride_glDeleteProgram

#	undef glBindTexture
#	define glBindTexture orion_gladoverride_glBindTexture

#	undef glDeleteTextures
#	define glDeleteTextures orion_gladoverride_glDeleteTextures
#endif

/** @endcond */

#ifdef __cplusplus
}
#endif

#endif // include guard