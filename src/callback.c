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

#include <stdio.h>
#include <string.h>

// ======================================================================================
// *****                           ORION DEFAULT CALLBACKS                          *****
// ======================================================================================

void _orionDefaultGLFWErrorCallback(int id, const char *msg) {
    printf("[Orion : WHOOPS] >> Error recieved from GLFW (error code %d): \"%s\"\n", id, msg);
    _orionThrowError(ORERR_GLFW_FAIL);
}

void _orionDefaultDebugMessageCallback(unsigned int src, unsigned int type, unsigned int id, unsigned int severity, int len, const char *msg, const void *param) {
    char srcstr[20];
    char typestr[25];
    char severitystr[15];

    switch (src) {
        case GL_DEBUG_SOURCE_API: strncpy(srcstr, "API", 20); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: strncpy(srcstr, "WINDOW_SYSTEM", 20); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: strncpy(srcstr, "SHADER_COMPILER", 20); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: strncpy(srcstr, "THIRD_PARTY", 20); break;
        case GL_DEBUG_SOURCE_APPLICATION: strncpy(srcstr, "APPLICATION", 20); break;
        case GL_DEBUG_SOURCE_OTHER: strncpy(srcstr, "OTHER", 20); break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR: strncpy(typestr, "ERROR", 25); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: strncpy(typestr, "DEPRECATED_BEHAVIOUR", 25); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: strncpy(typestr, "UNDEFINED_BEHAVIOUR", 25); break;
        case GL_DEBUG_TYPE_PORTABILITY: strncpy(typestr, "PORTABILITY", 25); break;
        case GL_DEBUG_TYPE_PERFORMANCE: strncpy(typestr, "PERFORMANCE", 25); break;
        case GL_DEBUG_TYPE_MARKER: strncpy(typestr, "MARKER", 25); break;
        case GL_DEBUG_TYPE_PUSH_GROUP: strncpy(typestr, "PUSH_GROUP", 25); break;
        case GL_DEBUG_TYPE_POP_GROUP: strncpy(typestr, "POP_GROUP", 25); break;
        case GL_DEBUG_TYPE_OTHER: strncpy(typestr, "OTHER", 25); break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: strncpy(severitystr, "HIGH", 15); break;
        case GL_DEBUG_SEVERITY_MEDIUM: strncpy(severitystr, "MEDIUM", 15); break;
        case GL_DEBUG_SEVERITY_LOW: strncpy(severitystr, "LOW", 15); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: strncpy(severitystr, "NOTIFICATION", 15); break;
    }

    printf("[Orion : DEBUG] >> GL error code %d : source %s : type %s : severity %s. See debugging message below:\n\t\t\"%s\"\n", id, srcstr, typestr, severitystr, msg);
}

// global callbacks
_orionCallbacks _oriCallbacks = {
    _orionDefaultGLFWErrorCallback,
    _orionDefaultDebugMessageCallback
};
