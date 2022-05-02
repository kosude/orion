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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ======================================================================================
// *****                          ORION INTERNAL DATA TYPES                         *****
// ======================================================================================

typedef struct _oriUniform {
    struct _oriUniform *next;

    int location;
    const char *name;
} _oriUniform;

// ======================================================================================
// *****                            ORION PUBLIC STRUCTURES                         *****
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
// *****                            ORION SHADER FUNCTIONS                          *****
// ======================================================================================

/**
 * @brief Allocate and initialise a new oriShader structure.
 * 
 */
oriShader *oriCreateShader() {
    _orionAssertVersion(200);

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
    oriShader *current = _orion.shaderListHead;
    while (current != shader)
        current = current->next;
    _orion.shaderListHead = shader->next;

    // opengl delete program
    glDeleteProgram(shader->handle);

    // free shader
    free(shader);
    shader = NULL;
}

/**
 * @brief Bind a shader struct.
 * 
 * @param shader the shader to bind.
 */
void oriBindShader(oriShader *shader) {
    _orionAssertVersion(200);

    if (oriCurrentShaderProgram() == shader->handle) {
        return;
    }
    glUseProgram(shader->handle);
}

/**
 * @brief Return the OpenGL handle to the given shader struct.
 * 
 * @param shader the shader to inspect.
 * 
 * @ingroup shaders
 */
unsigned int oriGetShaderHandle(oriShader *shader) {
    return shader->handle;
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

        // Log error to stdout
        // As string formatted is required here, printf is used instead of _orionThrowWarning.
        // This is annoying and be sure to change the style of this message if the style in _orionThrowWarning changes.
        printf("[Orion : WARN] >> %s\n", e);

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

    // initialise empty string to be returned in case of file read error.
    char *backup = malloc(sizeof(char));
    memset(backup, 0, sizeof(backup));

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        // 1 GB is a reasonable limit
        if (length > 1073741824) {
            _orionThrowWarning("(in oriParseShader()): The size of the specified source file exceeds the limit of 1 GB.");
            // free(buffer) is not called as buffer is not initialised so this would result in undefined behaviour
            return backup;
        }

        buffer = malloc((length + 1) * sizeof(char)); // +1 for null terminator.

        if (length) {
            readlength = fread(buffer, 1, length, f);

            if (length != readlength) {
                _orionThrowWarning("(in oriParseShader()): An error was encountered when reading the specified source file.");
                free(buffer);
                return backup;
            }
        }

        fclose(f);

        buffer[length] = '\0';
    } else {
        _orionThrowWarning("(in oriParseShader()): The specified source file could not be accessed.");
        // free(buffer) is not called as buffer is not initialised so this would result in undefined behaviour
        return backup;
    }

    free(backup);
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
            // As string formatted is required here, printf is used instead of _orionThrowWarning.
            // This is annoying and be sure to change the style of this message if the style in _orionThrowWarning changes.
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

// -----------------------
// oriSetUniform() stuff
// ----------------------

#define __setUniformHelper(glfunc, version) {\
    _orionAssertVersion(version);\
    unsigned int _boundCache = oriCurrentShaderProgram();\
    oriBindShader(shader);\
    glfunc;\
    glUseProgram(_boundCache);\
}

// ----------------
// oriSetUniform :: scalars

/** @ingroup shaders */ void oriSetUniform1i(oriShader *shader, const char *name, const int val) {
    __setUniformHelper(glUniform1i(oriShaderGetUniformLocation(shader, name), val), 200);
}
/** @ingroup shaders */ void oriSetUniform1f(oriShader *shader, const char *name, const float val) {
    __setUniformHelper(glUniform1f(oriShaderGetUniformLocation(shader, name), val), 200);
}
/** @ingroup shaders */ void oriSetUniform1ui(oriShader *shader, const char *name, const unsigned int val) {
    __setUniformHelper(glUniform1ui(oriShaderGetUniformLocation(shader, name), val), 300);
}

// ----------------
// oriSetUniform :: vectors

/** @ingroup shaders */ void oriSetUniform2i(oriShader *shader, const char *name, const int x, const int y) {
    __setUniformHelper(glUniform2i(oriShaderGetUniformLocation(shader, name), x, y), 200);
}
/** @ingroup shaders */ void oriSetUniform2f(oriShader *shader, const char *name, const float x, const float y) {
    __setUniformHelper(glUniform2f(oriShaderGetUniformLocation(shader, name), x, y), 200);
}
/** @ingroup shaders */ void oriSetUniform2ui(oriShader *shader, const char *name, const unsigned int x, const unsigned int y) {
    __setUniformHelper(glUniform2ui(oriShaderGetUniformLocation(shader, name), x, y), 300);
}

/** @ingroup shaders */ void oriSetUniform3i(oriShader *shader, const char *name, const int x, const int y, const int z) {
    __setUniformHelper(glUniform3i(oriShaderGetUniformLocation(shader, name), x, y, z), 200);
}
/** @ingroup shaders */ void oriSetUniform3f(oriShader *shader, const char *name, const float x, const float y, const float z) {
    __setUniformHelper(glUniform3f(oriShaderGetUniformLocation(shader, name), x, y, z), 200);
}
/** @ingroup shaders */ void oriSetUniform3ui(oriShader *shader, const char *name, const unsigned int x, const unsigned int y, const unsigned int z) {
    __setUniformHelper(glUniform3ui(oriShaderGetUniformLocation(shader, name), x, y, z), 300);
}

/** @ingroup shaders */ void oriSetUniform4i(oriShader *shader, const char *name, const int x, const int y, const int z, const int w) {
    __setUniformHelper(glUniform4i(oriShaderGetUniformLocation(shader, name), x, y, z, w), 200);
}
/** @ingroup shaders */ void oriSetUniform4f(oriShader *shader, const char *name, const float x, const float y, const float z, const float w) {
    __setUniformHelper(glUniform4f(oriShaderGetUniformLocation(shader, name), x, y, z, w), 200);
}
/** @ingroup shaders */ void oriSetUniform4ui(oriShader *shader, const char *name, const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int w) {
    __setUniformHelper(glUniform4ui(oriShaderGetUniformLocation(shader, name), x, y, z, w), 300);
}

// ----------------
// oriSetUniform :: matrices

/** @ingroup shaders */ void oriSetUniformMat2x2f(oriShader *shader, const char *name, const bool transpose, const float *mat) {
    __setUniformHelper(glUniformMatrix2fv(oriShaderGetUniformLocation(shader, name), 1, transpose, mat), 200);
}
/** @ingroup shaders */ void oriSetUniformMat2x3f(oriShader *shader, const char *name, const bool transpose, const float *mat) {
    __setUniformHelper(glUniformMatrix2x3fv(oriShaderGetUniformLocation(shader, name), 1, transpose, mat), 210);
}
/** @ingroup shaders */ void oriSetUniformMat2x4f(oriShader *shader, const char *name, const bool transpose, const float *mat) {
    __setUniformHelper(glUniformMatrix2x4fv(oriShaderGetUniformLocation(shader, name), 1, transpose, mat), 210);
}
/** @ingroup shaders */ void oriSetUniformMat3x2f(oriShader *shader, const char *name, const bool transpose, const float *mat) {
    __setUniformHelper(glUniformMatrix3x2fv(oriShaderGetUniformLocation(shader, name), 1, transpose, mat), 210);
}
/** @ingroup shaders */ void oriSetUniformMat3x3f(oriShader *shader, const char *name, const bool transpose, const float *mat) {
    __setUniformHelper(glUniformMatrix3fv(oriShaderGetUniformLocation(shader, name), 1, transpose, mat), 200);
}
/** @ingroup shaders */ void oriSetUniformMat3x4f(oriShader *shader, const char *name, const bool transpose, const float *mat) {
    __setUniformHelper(glUniformMatrix3x4fv(oriShaderGetUniformLocation(shader, name), 1, transpose, mat), 210);
}
/** @ingroup shaders */ void oriSetUniformMat4x2f(oriShader *shader, const char *name, const bool transpose, const float *mat) {
    __setUniformHelper(glUniformMatrix4x2fv(oriShaderGetUniformLocation(shader, name), 1, transpose, mat), 210);
}
/** @ingroup shaders */ void oriSetUniformMat4x3f(oriShader *shader, const char *name, const bool transpose, const float *mat) {
    __setUniformHelper(glUniformMatrix4x3fv(oriShaderGetUniformLocation(shader, name), 1, transpose, mat), 210);
}
/** @ingroup shaders */ void oriSetUniformMat4x4f(oriShader *shader, const char *name, const bool transpose, const float *mat) {
    __setUniformHelper(glUniformMatrix4fv(oriShaderGetUniformLocation(shader, name), 1, transpose, mat), 200);
}
