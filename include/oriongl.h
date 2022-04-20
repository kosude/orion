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

#include <glad/4.6/glad.h>

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
 * @brief Initialise the global (internal) Orion state and loads OpenGL functions.
 * @details This function is to be called @b after creating a window (either with Orion windows, raw GLFW, or another windowing library).
 * 
 * @param version the version of OpenGL that is being used.
 * 
 * @ingroup meta
 */
void oriInitialiseGL(const unsigned int version);

/**
 * @brief Terminate the Orion library. All Orion objects that were allocated will be freed.
 * 
 * @ingroup meta
 */
void oriTerminate();

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
 * 
 * @ingroup shaders
 */
void oriFreeShader(oriShader *shader);

#ifdef __cplusplus
}
#endif

#endif // include guard