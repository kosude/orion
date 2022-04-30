/* *************************************************************************************** */
/*                        ORION GRAPHICS LIBRARY AND RENDERING ENGINE                      */
/* *************************************************************************************** */
/* Copyright (c) 2022 Jack Bennett                                                         */
/* --------------------------------------------------------------------------------------- */
/* THE  SOFTWARE IS  PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND, EXPRESS  OR IMPLIED, */
/* INCLUDING  BUT  NOT  LIMITED  TO  THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS FOR  A */
/* PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN  NO EVENT SHALL  THE  AUTHORS  OR COPYRIGHT */
/* HOLDERS  BE  LIABLE  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF */
/* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR */
/* THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                              */
/* *************************************************************************************** */

#define ORIONGLAD_IMPLEMENTATION
#include "orionglad.h"

// ======================================================================================
// *****                   INTERNAL HELPER FUNCTIONS AND STRUCTURES                 *****
// ======================================================================================

/**
 * @brief a struct to hold the currently-bound GL buffer objects
 * 
 */
typedef struct {
    GLuint arrayBuffer;
    GLuint atomicCounterBuffer;
    GLuint copyReadBuffer;
    GLuint copyWriteBuffer;
    GLuint dispatchIndirectBuffer;
    GLuint drawIndirectBuffer;
    GLuint elementArrayBuffer;
    GLuint pixelPackBuffer;
    GLuint pixelUnpackBuffer;
    GLuint queryBuffer;
    GLuint shaderStorageBuffer;
    GLuint textureBuffer;
    GLuint transformFeedbackBuffer;
    GLuint uniformBuffer;
} _orionBoundBufferTypes;
_orionBoundBufferTypes _oriCurrentBuffers = { 0 };

/**
 * @brief a struct to hold the currently-bound GL texture objects
 * 
 */
typedef struct {
    GLuint t1d;
    GLuint t2d;
    GLuint t3d;
    GLuint t1dArray;
    GLuint t2dArray;
    GLuint rectangle;
    GLuint cubeMap;
    GLuint cubeMapArray;
    GLuint buffer;
    GLuint t2dMultisample;
    GLuint t2dMultisampleArray;
} _orionBoundTexTypes;
_orionBoundTexTypes _oriCurrentTextures = { 0 };

/**
 * @brief the currently-bound GL vertex array object
 */
GLuint _oriCurrentVertexArray = 0;

/**
 * @brief the currently-used GL shader program object
 */
GLuint _oriCurrentShaderProgram = 0;

/**
 * @brief return a pointer to a value in the public struct of currently-bound buffers that corresponds to the OpenGL equivalent \c (target).
 * @warning Be aware that this function can be very dangerous if not used properly: if the given target is not a valid OpenGL buffer target, \b a \b null \b pointer \b will \b be \b returned!
 * 
 * @param target the OpenGL target
 */
GLuint *_oriCurrentBufferPtrAt(GLenum target) {
    switch (target) {
        case GL_ARRAY_BUFFER:
            return &(_oriCurrentBuffers.arrayBuffer);
        case GL_ATOMIC_COUNTER_BUFFER:
            return &(_oriCurrentBuffers.atomicCounterBuffer);
        case GL_COPY_READ_BUFFER:
            return &(_oriCurrentBuffers.copyReadBuffer);
        case GL_COPY_WRITE_BUFFER:
            return &(_oriCurrentBuffers.copyWriteBuffer);
        case GL_DISPATCH_INDIRECT_BUFFER:
            return &(_oriCurrentBuffers.dispatchIndirectBuffer);
        case GL_DRAW_INDIRECT_BUFFER:
            return &(_oriCurrentBuffers.drawIndirectBuffer);
        case GL_ELEMENT_ARRAY_BUFFER:
            return &(_oriCurrentBuffers.elementArrayBuffer);
        case GL_PIXEL_PACK_BUFFER:
            return &(_oriCurrentBuffers.pixelPackBuffer);
        case GL_PIXEL_UNPACK_BUFFER:
            return &(_oriCurrentBuffers.pixelUnpackBuffer);
        case GL_QUERY_BUFFER:
            return &(_oriCurrentBuffers.queryBuffer);
        case GL_SHADER_STORAGE_BUFFER:
            return &(_oriCurrentBuffers.shaderStorageBuffer);
        case GL_TEXTURE_BUFFER:
            return &(_oriCurrentBuffers.textureBuffer);
        case GL_TRANSFORM_FEEDBACK_BUFFER:
            return &(_oriCurrentBuffers.transformFeedbackBuffer);
        case GL_UNIFORM_BUFFER:
            return &(_oriCurrentBuffers.uniformBuffer);
        default:
            return 0;
    }
}

/**
 * @brief return a pointer to a value in the public struct of currently-bound textures that corresponds to the OpenGL equivalent \c (target).
 * @warning Be aware that this function can be very dangerous if not used properly: if the given target is not a valid OpenGL texture target, \b a \b null \b pointer \b will \b be \b returned!
 * 
 * @param target the OpenGL target
 */
GLuint *_oriCurrentTexturePtrAt(GLenum target) {
    switch (target) {
        case GL_TEXTURE_1D:
            return &(_oriCurrentTextures.t1d);
        case GL_TEXTURE_2D:
            return &(_oriCurrentTextures.t2d);
        case GL_TEXTURE_3D:
            return &(_oriCurrentTextures.t3d);
        case GL_TEXTURE_1D_ARRAY:
            return &(_oriCurrentTextures.t1dArray);
        case GL_TEXTURE_2D_ARRAY:
            return &(_oriCurrentTextures.t2dArray);
        case GL_TEXTURE_RECTANGLE:
            return &(_oriCurrentTextures.rectangle);
        case GL_TEXTURE_CUBE_MAP:
            return &(_oriCurrentTextures.cubeMap);
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            return &(_oriCurrentTextures.cubeMapArray);
        case GL_TEXTURE_BUFFER:
            return &(_oriCurrentTextures.buffer);
        case GL_TEXTURE_2D_MULTISAMPLE:
            return &(_oriCurrentTextures.t2dMultisample);
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            return &(_oriCurrentTextures.t2dMultisample);
        default:
            return 0;
    }
}

// ======================================================================================
// *****                          PUBLIC ORIONGLAD INTERFACE                        *****
// ======================================================================================

// ======================================================================================
// *****                           NEW / ADDED FUNCTIONALITY                        *****
// ======================================================================================

// ======================================================================================
// *****                        ADDED FUNCTIONALITY :: BUFFERS                      *****
// ======================================================================================

/**
 * @brief the current GL object that is bound to \c target
 * @details It is recommended to refer to this as opposed to calling glGetx functions for better performance.
 * 
 * @param target the target to query
 * 
 * @ingroup orionglad
 */
const GLuint orion_glCurrentBufferAt(GLenum target) {
    if (!_oriCurrentBufferPtrAt(target)) {
        return 0;
    }
    return *(_oriCurrentBufferPtrAt(target));
}

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
const GLenum orion_glGetBufferTarget(GLuint buffer) {
    // unfortunately if-else has to be used here as the queried values are not constant
    // otherwise, I would normally use switch-case.
    if (buffer == _oriCurrentBuffers.arrayBuffer) {
        return GL_ARRAY_BUFFER;
    } else if (buffer == _oriCurrentBuffers.atomicCounterBuffer) {
        return GL_ATOMIC_COUNTER_BUFFER;
    } else if (buffer == _oriCurrentBuffers.copyReadBuffer) {
        return GL_COPY_READ_BUFFER;
    } else if (buffer == _oriCurrentBuffers.copyWriteBuffer) {
        return GL_COPY_WRITE_BUFFER;
    } else if (buffer == _oriCurrentBuffers.dispatchIndirectBuffer) {
        return GL_DISPATCH_INDIRECT_BUFFER;
    } else if (buffer == _oriCurrentBuffers.drawIndirectBuffer) {
        return GL_DRAW_INDIRECT_BUFFER;
    } else if (buffer == _oriCurrentBuffers.elementArrayBuffer) {
        return GL_ELEMENT_ARRAY_BUFFER;
    } else if (buffer == _oriCurrentBuffers.pixelPackBuffer) {
        return GL_PIXEL_PACK_BUFFER;
    } else if (buffer == _oriCurrentBuffers.pixelUnpackBuffer) {
        return GL_PIXEL_UNPACK_BUFFER;
    } else if (buffer == _oriCurrentBuffers.queryBuffer) {
        return GL_QUERY_BUFFER;
    } else if (buffer == _oriCurrentBuffers.shaderStorageBuffer) {
        return GL_SHADER_STORAGE_BUFFER;
    } else if (buffer == _oriCurrentBuffers.textureBuffer) {
        return GL_TEXTURE_BUFFER;
    } else if (buffer == _oriCurrentBuffers.transformFeedbackBuffer) {
        return GL_TRANSFORM_FEEDBACK_BUFFER;
    } else if (buffer == _oriCurrentBuffers.uniformBuffer) {
        return GL_UNIFORM_BUFFER;
    } else {
        // buffer is not bound
        return 0;
    }
}

// ======================================================================================
// *****                       ADDED FUNCTIONALITY :: TEXTURES                      *****
// ======================================================================================

/**
 * @brief the current GL texture that is bound to \c target
 * @details It is recommended to refer to this as opposed to calling glGetx functions for better performance.
 * 
 * @param target the target to query
 * 
 * @ingroup orionglad
 */
const GLuint orion_glCurrentTextureAt(GLenum target) {
    if (!_oriCurrentTexturePtrAt(target)) {
        return 0;
    }
    return *(_oriCurrentTexturePtrAt(target));
}
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
const GLenum orion_glGetTextureTarget(GLuint tex) {
    // unfortunately if-else has to be used here as the queried values are not constant
    // otherwise, I would normally use switch-case.
    if (tex == _oriCurrentTextures.t1d) {
        return GL_TEXTURE_1D;
    } else if (tex == _oriCurrentTextures.t2d) {
        return GL_TEXTURE_2D;
    } else if (tex == _oriCurrentTextures.t3d) {
        return GL_TEXTURE_3D;
    } else if (tex == _oriCurrentTextures.t1dArray) {
        return GL_TEXTURE_1D_ARRAY;
    } else if (tex == _oriCurrentTextures.t2dArray) {
        return GL_TEXTURE_2D_ARRAY;
    } else if (tex == _oriCurrentTextures.rectangle) {
        return GL_TEXTURE_RECTANGLE;
    } else if (tex == _oriCurrentTextures.cubeMap) {
        return GL_TEXTURE_CUBE_MAP;
    } else if (tex == _oriCurrentTextures.cubeMapArray) {
        return GL_TEXTURE_CUBE_MAP_ARRAY;
    } else if (tex == _oriCurrentTextures.buffer) {
        return GL_TEXTURE_BUFFER;
    } else if (tex == _oriCurrentTextures.t2dMultisample) {
        return GL_TEXTURE_2D_MULTISAMPLE;
    } else if (tex == _oriCurrentTextures.t2dMultisampleArray) {
        return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
    } else {
        // texture is not bound
        return 0;
    }
}

// ======================================================================================
// *****                    ADDED FUNCTIONALITY :: VERTEX ARRAYS                    *****
// ======================================================================================

/**
 * @brief the currently-bound GL vertex array object
 * @details It is recommended to refer to this as opposed to calling glGetx functions for better performance.
 * 
 * @ingroup orionglad
 */
const GLuint orion_glCurrentVertexArray() {
    return _oriCurrentVertexArray;
}

// ======================================================================================
// *****                      ADDED FUNCTIONALITY :: SHADERS                        *****
// ======================================================================================

/**
 * @brief the currently-used GL shader program object
 * @details It is recommended to refer to this as opposed to calling glGetx functions for better performance.
 * 
 * @ingroup orionglad
 */
const GLuint orion_glCurrentShaderProgram() {
    return _oriCurrentShaderProgram;
}

// ======================================================================================
// *****                      OVERRIDES OF EXISTING GL FUNCTIONS                    *****
// ======================================================================================

// ======================================================================================
// *****                             OVERRIDES :: BUFFERS                           *****
// ======================================================================================

/**
 * @brief binds a GL buffer object of name \c buffer to \c target
 * 
 * @param target specifies the target to which the buffer point is bound
 * @param buffer specifies the name of a buffer object
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glBindBuffer(GLenum target, GLuint buffer) {
    if (!_oriCurrentBufferPtrAt(target)) {
        return;
    }
    *(_oriCurrentBufferPtrAt(target)) = buffer;

    glBindBuffer(target, buffer);
}

/**
 * @brief deletes named buffer objects
 * 
 * @param n the number of buffer objects to be deleted
 * @param buffers specifies an array of buffer objects to be deleted
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glDeleteBuffers(GLsizei n, const GLuint *buffers) {
    for (unsigned int i = 0; i < n; i++) {
        // get the target of each buffer
        GLenum _target = orion_glGetBufferTarget(buffers[i]);

        // make sure target is valid (it should be anyway, but better safe than sorry)
        if (_oriCurrentBufferPtrAt(_target) == 0) {
            return;
        }

        // set the corresponding CurrentBuffer value to 0 if the deleted buffer was bound
        // this mimics OpenGL's behaviour: the target of the given buffer is set to 0 (absence of buffer) if it was bound
        if (*(_oriCurrentBufferPtrAt(_target)) == buffers[i]) {
            *(_oriCurrentBufferPtrAt(_target)) = 0;
        }
    }

    glDeleteBuffers(n, buffers);
}

// ======================================================================================
// *****                            OVERRIDES :: TEXTURES                           *****
// ======================================================================================

/**
 * @brief bind a named texture to a texturing target
 * 
 * @param target specifies the target to which the texture is bound
 * @param texture specifies the name of a texture.
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glBindTexture(GLenum target, GLuint texture) {
    if (!_oriCurrentTexturePtrAt(target)) {
        return;
    }
    *(_oriCurrentTexturePtrAt(target)) = texture;

    glBindTexture(target, texture);
}

/**
 * @brief deletes named textures
 * 
 * @param n the number of texturess to be deleted
 * @param textures specifies an array of textures to be deleted
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glDeleteTextures(GLsizei n, const GLuint *textures) {
    for (unsigned int i = 0; i < n; i++) {
        // get the target of each buffer
        GLenum _target = orion_glGetTextureTarget(textures[i]);

        // make sure target is valid (it should be anyway, but better safe than sorry)
        if (_oriCurrentTexturePtrAt(_target) == 0) {
            return;
        }

        // set the corresponding CurrentBuffer value to 0 if the deleted buffer was bound
        // this mimics OpenGL's behaviour: the target of the given buffer is set to 0 (absence of buffer) if it was bound
        if (*(_oriCurrentTexturePtrAt(_target)) == textures[i]) {
            *(_oriCurrentTexturePtrAt(_target)) = 0;
        }
    }

    glDeleteTextures(n, textures);
}

// ======================================================================================
// *****                         OVERRIDES :: VERTEX ARRAYS                         *****
// ======================================================================================

/**
 * @brief binds a GL vertex array object of name \c array
 * 
 * @param array the name of the vertex array
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glBindVertexArray(GLuint array) {
    _oriCurrentVertexArray = array;
    glBindVertexArray(array);
}

/**
 * @brief deletes named vertex array objects
 * 
 * @param n the number of vertex array objects to be deleted
 * @param arrays specifies an array of vertex array objects to be deleted
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glDeleteVertexArrays(GLsizei n, const GLuint *arrays) {
    for (unsigned int i = 0; i < n; i++) {
        // if the vao was bound, set the current bound vao to 0
        if (_oriCurrentVertexArray == arrays[i]) { 
            _oriCurrentVertexArray = 0;
        }
    }

    glDeleteVertexArrays(n, arrays);
}

// ======================================================================================
// *****                             OVERRIDES :: SHADERS                           *****
// ======================================================================================

/**
 * @brief installs a program object as part of current rendering state
 * 
 * @param program specifies the handle of the program object whose executables are to be used as part of current rendering state.
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glUseProgram(GLuint program) {
    _oriCurrentShaderProgram = program;
    glUseProgram(program);
}

/**
 * @brief deletes a GL shader program object
 * 
 * @param program specifies the program object to be deleted.
 * 
 * @ingroup orionglad
 */
void orion_gladoverride_glDeleteProgram(GLuint program) {
    if (_oriCurrentShaderProgram == program) {
        _oriCurrentShaderProgram = 0;
    }
    glDeleteProgram(program);
}