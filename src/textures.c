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

#include "internal.h"
#include "oriongl.h"

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

// ======================================================================================
// *****                            ORION PUBLIC STRUCTURES                         *****
// ======================================================================================

typedef struct oriTexture {
    oriTexture *next;
    unsigned int handle;

    unsigned int type;
    unsigned int width;
    unsigned int height;
    unsigned int depth;
    unsigned int internalFormat;
    unsigned int levels;
    unsigned int samples;

    bool immutableStorage;
} oriTexture;

// ======================================================================================
// *****                           ORION TEXTURE FUNCTIONS                          *****
// ======================================================================================

/**
 * @brief Allocate and initialise a new oriTexture structure with mutable storage.
 *
 * @details As told in the OpenGL specification, textures cannot be rebound to different targets:
 *  > It is not legal to bind a [texture object] to a different target than the one it was previously bound with.
 *  > So if you generate a texture and bind it as GL_TEXTURE_1D, then you must continue to bind it as such.
 * Therefore, the binding target is specified in this function (instead of the bind function) so that it is not
 * possible for it to be accidentally bound differently in the future.
 *
 * @param target the target to bind to.
 * @param internalFormat the internal format of the texture.
 *
 * @ingroup textures
 */
oriTexture *oriCreateTexture(unsigned int target, unsigned int internalFormat) {
    // assert correct version
    _orionAssertVersion(200);

    // assert version 3.2 or higher if necessary
    if (target == GL_TEXTURE_2D_MULTISAMPLE || target == GL_TEXTURE_2D_MULTISAMPLE_ARRAY) {
        _orionAssertVersion(320);
    }

    oriTexture *r = malloc(sizeof(oriTexture));
    r->type = target;
    r->width = 0;
    r->height = 0;
    r->depth = 0;
    r->internalFormat = internalFormat;
    r->levels = 0;
    r->samples = 0;
    r->immutableStorage = false;

    // use DSA if possible
    if (_orion.glVersion >= 450) {
        glCreateTextures(target, 1, &r->handle);
    } else {
        glGenTextures(1, &r->handle);
    }

    // push to global linked list
    r->next = _orion.textureListHead;
    _orion.textureListHead = r;

    return r;
}

/**
 * @brief Allocate and initialise a new oriTexture structure with immutable storage.
 *
 * @sa oriCreateTexture()
 * @sa <a href="https://www.khronos.org/opengl/wiki/Texture_Storage#Immutable_storage">OpenGL/Immutable texture storage</a>
 *
 * @param target the target to bind to.
 * @param width the width of the texture.
 * @param height the height of the texture. Set to 0 if not applicable.
 * @param depth the depth of the texture. Set to 0 if not applicable.
 * @param levels the amount of texture levels. Set to 0 if not applicable.
 * @param internalFormat the internal format with which texture image data will be stored, e.g. @c GL_RGBA8.
 * @param samples the number of samples in the texture. Set to 0 if not applicable.
 * @param fixedSampleLocation set to true if the image will use identical sample locations and the same number of samples for all texels in the image, and the sample
 * locations will not depend on the internal format or size of the image.
 *
 * @sa <a href="https://docs.gl/gl4/glTexStorage1D">glTexStorage1D</a>
 * @sa <a href="https://docs.gl/gl4/glTexStorage2D">glTexStorage2D</a>
 * @sa <a href="https://docs.gl/gl4/glTexStorage3D">glTexStorage3D</a>
 * @sa <a href="https://docs.gl/gl4/glTexStorage2DMultisample">glTexStorage2DMultisample</a>
 * @sa <a href="https://docs.gl/gl4/glTexStorage3DMultisample">glTexStorage3DMultisample</a>
 *
 * @ingroup textures
 */
oriTexture *oriCreateTextureImmutable(unsigned int target, unsigned int width, unsigned int height, unsigned int depth, unsigned int internalFormat, unsigned int levels, unsigned int samples, bool fixedSampleLocations) {
    _orionAssertVersion(420);

    oriTexture *r = oriCreateTexture(target, internalFormat);
    r->immutableStorage = true;

    // get the appropriate glTexStorage* function to call.
    // (or glTextureStorage* if DSA is possible i.e. version is >= 450)

    // 0: glTexStorage1D
    // 1: glTexStorage2D
    // 2: glTexStorage3D
    // 3: glTexStorage2DMultisample
    // 4: glTexStorage3DMultisample
    unsigned int glTexStorageFuncType;

    switch (r->type) {
        case GL_TEXTURE_1D:
            glTexStorageFuncType = 0;
            break;
        case GL_TEXTURE_2D:
        case GL_TEXTURE_RECTANGLE:
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_1D_ARRAY:
            glTexStorageFuncType = 1;
            break;
        case GL_TEXTURE_3D:
        case GL_TEXTURE_2D_ARRAY:
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            glTexStorageFuncType = 2;
            break;
        case GL_TEXTURE_2D_MULTISAMPLE:
            glTexStorageFuncType = 3;
            return r;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            glTexStorageFuncType = 4;
            return r;
        default:
            _orionThrowWarning("(in oriCreateTextureImmutable()): Unsupported texture type specified. Immutable texture storage not allocated.");
            return r;
    }

    // bind to this again at the end of the function if DSA is not used.
    unsigned int boundCache;

    if (_orion.glVersion < 450) {
        boundCache = oriCurrentTextureAt(r->type);
        glBindTexture(r->type, r->handle);
    }

    // NULL = 0 so this should work fine even if NULL is passed as any parameters.
    // I don't think fixedSampleLocations needs to be stored?
    r->width = width;
    r->height = height;
    r->depth = depth;
    r->levels = levels;
    r->samples = samples;

    // if required parameters are set to NULL then OpenGL won't be very happy.
    // frankly, this function is convoluted enough as is. It's the user's responsibility to have common sense and give the required arguments, so..
    // ..we're not checking for those parameters. Too bad!

    switch (glTexStorageFuncType) {
        case 0:
            if (_orion.glVersion >= 450) {
                glTextureStorage1D(r->handle, levels, internalFormat, width);
            } else {
                glTexStorage1D(r->type, levels, internalFormat, width);
            }
            break;
        case 1:
            if (_orion.glVersion >= 450) {
                glTextureStorage2D(r->handle, levels, internalFormat, width, height);
            } else {
                glTexStorage2D(r->type, levels, internalFormat, width, height);
            }
            break;
        case 2:
            if (_orion.glVersion >= 450) {
                glTextureStorage3D(r->handle, levels, internalFormat, width, height, depth);
            } else {
                glTexStorage3D(r->type, levels, internalFormat, width, height, depth);
            }
            break;
        case 3:
            if (_orion.glVersion >= 450) {
                glTextureStorage2DMultisample(r->handle, samples, internalFormat, width, height, fixedSampleLocations);
            } else {
                glTexStorage2DMultisample(r->type, samples, internalFormat, width, height, fixedSampleLocations);
            }
            break;
        case 4:
            if (_orion.glVersion >= 450) {
                glTextureStorage3DMultisample(r->handle, samples, internalFormat, width, height, depth, fixedSampleLocations);
            } else {
                glTexStorage3DMultisample(r->type, samples, internalFormat, width, height, depth, fixedSampleLocations);
            }
            break;
    }

    // don't affect global state outside of this function
    if (_orion.glVersion < 450) {
        glBindTexture(r->type, boundCache);
    }

    return r;
}

/**
 * @brief Destroy and free memory for the given texture.
 *
 * @param texture the texture to free.
 *
 * @ingroup textures
 */
void oriFreeTexture(oriTexture *texture) {
    _orionAssertVersion(200);

    // unlink from global linked list.
    oriTexture *current = _orion.textureListHead;
    while (current != texture) {
        current = current->next;
    }
    _orion.textureListHead = texture->next;

    glDeleteTextures(1, &texture->handle);

    free(texture);
    texture = NULL;
}

/**
 * @brief Bind a given texture to the specified target.
 *
 * @param texture the texture to bind.
 * @param unit the texture image unit to bind the texture to.
 *
 * @ingroup textures
 */
void oriBindTexture(oriTexture *texture, unsigned int unit) {
    _orionAssertVersion(200);

    if (oriCurrentTextureAt(texture->type) == texture->handle) {
        return;
    }

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(texture->type, texture->handle);
}

/**
 * @brief Return the OpenGL handle to the given texture structure.
 *
 * @param texture the texture to inspect.
 *
 * @ingroup textures
 */
unsigned int oriGetTextureHandle(oriTexture *texture) {
    return texture->handle;
}

/**
 * @brief Return texture properties into the specified variables.
 * 
 * @details If you don't want to recieve a property, pass NULL as the argument.
 * 
 * @param texture the texture to inspect.
 * @param type the type of the texture, e.g. @c GL_TEXTURE_2D, or @c GL_TEXTURE_CUBE_MAP.
 * @param width the width of the texture's base mipmap level image.
 * @param height the height of the texture's base mipmap level image; only applicable when the texture's image is not 1-dimensional.
 * @param depth the depth of the texture's base mipmap level image; only applicable if the texture is @c GL_TEXTURE_3D.
 * @param format the internal format of the texture.
 * @param levels the amount of levels in the texture.
 * @param samples the amount of samples in the texture; only applicable if the texture is multisampled.
 * 
 * @ingroup textures
 */
void oriGetTextureProperty(oriTexture *texture, unsigned int *type, unsigned int *width, unsigned int *height, unsigned int *depth, unsigned int *format, unsigned int *levels, unsigned int *samples) {
    if (type) *type = texture->type;
    if (width) *width = texture->width;
    if (height) *height = texture->height;
    if (depth) *depth = texture->depth;
    if (format) *format = texture->internalFormat;
    if (levels) *levels = texture->levels;
    if (samples) *samples = texture->samples;
}

/**
 * @brief Fill the given texture's storage with an image at the specified path.
 * 
 * @details The path is relative to the location of the executable.
 * 
 * @param texture the texture object to update.
 * @param dataType the GL type of the given data (e.g. GL_UNSIGNED_BYTE if @c data is an unsigned char array)
 * @param data the image data to use.
 * @param width the width of the desired image.
 * @param height the height of the desired image. Set to 0 if the texture is 1D.
 * @param depth the depth of the texture. Set to 0 if the texture is not 3D.
 * @param imageFormat the format of the image to be loaded.
 * 
 * @ingroup textures
 */
void oriUploadTexImage(oriTexture *texture, unsigned int dataType, const void *data, unsigned int width, unsigned int height, unsigned int depth, unsigned int imageFormat) {
    _orionAssertVersion(200);

    // get the appropriate glTexImage* / glTexSubImage* function to call.
    // (or glTextureSubImage* if DSA is possible i.e. version is >= 450)

    // 0: glTex*Image1D
    // 1: glTex*Image2D
    // 2: glTex*Image3D
    unsigned int glTexImageFuncType;

    switch (texture->type) {
        case GL_TEXTURE_1D:
            glTexImageFuncType = 0;
            break;
        case GL_TEXTURE_2D:
        case GL_TEXTURE_RECTANGLE:
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_1D_ARRAY:
            glTexImageFuncType = 1;
            break;
        case GL_TEXTURE_3D:
        case GL_TEXTURE_2D_ARRAY:
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            glTexImageFuncType = 2;
            return;
        case GL_TEXTURE_2D_MULTISAMPLE:
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            _orionThrowWarning("(in oriUploadTexImage()): OpenGL does not support directly writing to multisample textures. Texture data not updated.");
            return;
        default:
            _orionThrowWarning("(in oriUploadTexImage()): Unsupported texture type specified. Texture data not updated.");
            return;
    }

    // bind to this again at the end of the function if DSA is not used.
    unsigned int boundCache;

    if (_orion.glVersion < 450) {
        boundCache = oriCurrentTextureAt(texture->type);
        glBindTexture(texture->type, texture->handle);
    }

    if (texture->immutableStorage) {
        switch (glTexImageFuncType) {
            case 0:
                if (_orion.glVersion >= 450) {
                    glTextureSubImage1D(texture->handle, 0, 0, texture->width, imageFormat, dataType, data);
                } else {
                    glTexSubImage1D(texture->type, 0, 0, texture->width, imageFormat, dataType, data);
                }

                break;
            case 1:
                if (_orion.glVersion >= 450) {
                    glTextureSubImage2D(texture->handle, 0, 0, 0, texture->width, texture->height, imageFormat, dataType, data);
                } else {
                    glTexSubImage2D(texture->type, 0, 0, 0, texture->width, texture->height, imageFormat, dataType, data);
                }

                break;
            case 2:
                if (_orion.glVersion >= 450) {
                    glTextureSubImage3D(texture->handle, 0, 0, 0, 0, texture->width, texture->height, texture->depth, imageFormat, dataType, data);
                } else {
                    glTexSubImage3D(texture->type, 0, 0, 0, 0, texture->width, texture->height, texture->depth, imageFormat, dataType, data);
                }

                break;
        }
    } else {
        // *** In this case, space for the texture must be reallocated. *** //

        // there is no DSA version of these functions.
        if (_orion.glVersion >= 450) {
            // bind if not already bound
            glBindTexture(texture->type, texture->handle);
        }

        switch (glTexImageFuncType) {
            case 0:
                glTexImage1D(texture->type, 0, texture->internalFormat, width, 0, imageFormat, dataType, data);

                // update texture properties as the texture storage has been reallocated.
                texture->width = width;
                texture->height = 0;
                texture->depth = 0;

                break;
            case 1:
                glTexImage2D(texture->type, 0, texture->internalFormat, width, height, 0, imageFormat, dataType, data);

                // update texture properties as the texture storage has been reallocated.
                texture->width = width;
                texture->height = height;
                texture->depth = 0;

                break;
            case 2:
                glTexImage3D(texture->type, 0, texture->internalFormat, width, height, depth, 0, imageFormat, dataType, data);

                // update texture properties as the texture storage has been reallocated.
                texture->width = width;
                texture->height = height;
                texture->depth = depth;

                break;
        }
    }

    // don't affect global state outside of this function
    if (_orion.glVersion < 450) {
        // (generate mipmaps too)
        glGenerateMipmap(texture->type);

        glBindTexture(texture->type, boundCache);
    } else {
        // generate mipmap with DSA
        glGenerateTextureMipmap(texture->handle);
    }
}

/**
 * @brief Set a parameter for the given texture.
 *
 * @param texture the texture to update.
 * @param param the texture parameter to set.
 * @param val the value to set the parameter to.
 *
 * @ingroup textures
 */
void oriSetTextureParameteri(oriTexture *texture, unsigned int param, int val) {
    _orionAssertVersion(200);

    if (_orion.glVersion >= 450) {
        glTextureParameteri(texture->handle, param, val);
    } else {
        unsigned int boundCache = oriCurrentTextureAt(texture->type);
        glBindTexture(texture->type, texture->handle);

        glTexParameteri(texture->type, param, val);

        glBindTexture(texture->type, boundCache);
    }
}

/**
 * @brief Set a parameter for the given texture.
 *
 * @param texture the texture to update.
 * @param param the texture parameter to set.
 * @param val the value to set the parameter to.
 *
 * @ingroup textures
 */
void oriSetTextureParameterf(oriTexture *texture, unsigned int param, float val) {
    _orionAssertVersion(200);

    if (_orion.glVersion >= 450) {
        glTextureParameterf(texture->handle, param, val);
    } else {
        unsigned int boundCache = oriCurrentTextureAt(texture->type);
        glBindTexture(texture->type, texture->handle);

        glTexParameterf(texture->type, param, val);

        glBindTexture(texture->type, boundCache);
    }
}

/**
 * @brief Return the value of the specified texture parameter for the given texture object.
 *
 * @param texture the texture to inspect.
 * @param param the texture parameter to check.
 * @param out a pointer to the output variable.
 *
 * @ingroup textures
 */
int oriGetTextureParameteri(oriTexture *texture, unsigned int param) {
    _orionAssertVersion(200);

    int r;
    if (_orion.glVersion >= 450) {
        glGetTextureParameteriv(texture->handle, param, &r);
    } else {
        unsigned int boundCache = oriCurrentTextureAt(texture->type);
        glBindTexture(texture->type, texture->handle);

        glGetTexParameteriv(texture->type, param, &r);

        glBindTexture(texture->type, boundCache);
    }

    return r;
}

/**
 * @brief Return the value of the specified texture parameter for the given texture object.
 *
 * @param texture the texture to inspect.
 * @param param the texture parameter to check.
 * @param out a pointer to the output variable.
 *
 * @ingroup textures
 */
float oriGetTextureParameterf(oriTexture *texture, unsigned int param) {
    _orionAssertVersion(200);

    float r;
    if (_orion.glVersion >= 450) {
        glGetTextureParameterfv(texture->handle, param, &r);
    } else {
        unsigned int boundCache = oriCurrentTextureAt(texture->type);
        glBindTexture(texture->type, texture->handle);

        glGetTexParameterfv(texture->type, param, &r);

        glBindTexture(texture->type, boundCache);
    }

    return r;
}
