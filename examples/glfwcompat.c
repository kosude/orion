#include "oriongl.h"

#include <glfw3.h>
#include <zetaml.h>

// ORION GRAPHICS LIBRARY DEMO
// 2022 Jack Bennett

/*========================================================================*/
/* This example file demonstrates the use of Orion functions alongside a  */
/* third-party windowing API - in this case, GLFW. Orion objects, such as */
/* vertex buffers and shaders, can be used as normal.                     */
/*========================================================================*/

float vertices[] = {
    -0.5f, -0.5f, 0.0f,		1, 0, 0, 1,
     0.5f, -0.5f, 0.0f,		0, 1, 0, 1,
    -0.5f,  0.5f, 0.0f,		0, 0, 1, 1,
     0.5f,  0.5f, 0.0f,		1, 1, 0, 1
};

unsigned int indices[] = {
    0, 1, 2,
    3, 1, 2
};

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window = glfwCreateWindow(300, 300, "Orion with GLFW", NULL, NULL);
    glfwMakeContextCurrent(window);

    oriLoadGL((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    oriInitialise(460);
    oriEnableDebugContext(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, GL_DONT_CARE, false, NULL, 0);

    oriBuffer *ibo = oriCreateBuffer();
    oriSetBufferData(ibo, indices, sizeof(indices), GL_STATIC_DRAW);

    oriBuffer *vbo = oriCreateBuffer();
    oriSetBufferData(vbo, vertices, sizeof(vertices), GL_STATIC_DRAW);
    oriVertexArray *vao = oriCreateVertexArray();
    oriSpecifyVertexData(vao, vbo, 0, 3, GL_FLOAT, false, 7 * sizeof(float), 0 * sizeof(float)); // vertex positions
    oriSpecifyVertexData(vao, vbo, 2, 4, GL_FLOAT, false, 7 * sizeof(float), 3 * sizeof(float)); // vertex colours

    oriShader *shader = oriCreateShader();
    oriAddShaderSource(shader, GL_VERTEX_SHADER, ORION_VERTEX_SHADER_BASIC);
    oriAddShaderSource(shader, GL_FRAGMENT_SHADER, ORION_FRAGMENT_SHADER_BASIC);

    zmlMatrix mat = zmlIdentityMatrix(4, 4);
    zmlVector vec = zmlConstructVectorDefault(3, 1.5);
    zmlScale(&mat, vec);
    float matelem[4][4];
    zmlCopyMatrixElements(mat, matelem);
    oriSetUniformMat4x4f(shader, "transform.mvp", false, &matelem[0][0]);

    oriSetUniform1i(shader, "blend.mode", 3);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        oriBindVertexArray(vao);
        oriBindBuffer(ibo, GL_ELEMENT_ARRAY_BUFFER);
        oriBindShader(shader);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    zmlFreeMatrix(&mat);
    zmlFreeVector(&vec);

    oriTerminate();
    glfwTerminate();
    return 0;
}
