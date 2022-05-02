#include "oriongl.h"    // base graphics library

#include <glfw3.h>      // the GLFW API is used here (any other windowing API can be used, though)
#include <zetaml.h>     // the Zeta maths library is used for transformations

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
    // Initialise GLFW
    glfwInit();

    // Set GLFW window hints:
    //    context version and profile, resizable property to false.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    // Create a GLFW window (note the absence of Orion functions up to this point)
    GLFWwindow *window = glfwCreateWindow(300, 300, "Orion with GLFW", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Load GL functions:
    //    this is not necessary when using Orionwin as it is done in the creation of the window automatically.
    //    (note it depends on the current context, so the window's context must be current at this point)
    oriLoadGL((GLADloadproc) glfwGetProcAddress);

    // Initialise the Orion state, using the same version as specified to GLFW.
    oriInitialise(460);

    // Create an buffer to store the indices of the square.
    oriBuffer *ibo = oriCreateBuffer();
    oriSetBufferData(ibo, indices, sizeof(indices), GL_STATIC_DRAW);

    // Create an buffer to store the vertices of the square.
    oriBuffer *vbo = oriCreateBuffer();
    oriSetBufferData(vbo, vertices, sizeof(vertices), GL_STATIC_DRAW);

    // Create a vertex array object to associate the new vertex buffer with the format of the data
    oriVertexArray *vao = oriCreateVertexArray();
    // The VBO is associated with the VAO in these function calls.
    oriSpecifyVertexData(vao, vbo, 0, 3, GL_FLOAT, false, 7 * sizeof(float), 0 * sizeof(float)); // vertex positions
    oriSpecifyVertexData(vao, vbo, 2, 4, GL_FLOAT, false, 7 * sizeof(float), 3 * sizeof(float)); // vertex colours

    // A shader program is created, using the built-in basic shader presets.
    oriShader *shader = oriCreateShader();
    oriAddShaderSource(shader, GL_VERTEX_SHADER, ORION_VERTEX_SHADER_BASIC);
    oriAddShaderSource(shader, GL_FRAGMENT_SHADER, ORION_FRAGMENT_SHADER_BASIC);

    // A blend mode of 3 is used so that the colour of the shape takes that of the given vertex colours, specified above.
    // See the source code of the basic shader presets for more info on the available blend modes.
    oriSetUniform1i(shader, "blend.mode", 3);

    // An identity matrix is used as the basis for the MVP so that vertices will be rendered.
    zmlMatrix mat = zmlIdentityMatrix(4, 4);

    // This identity matrix is scaled by 1.5 so that the resulting square is 50% bigger than its original size.
    zmlVector vec = zmlConstructVectorDefault(3, 1.5);
    zmlScale(&mat, vec);

    // The contents of the resulting model matrix are copied onto the stack in the 2D array 'matelem'.
    float matelem[4][4];
    zmlCopyMatrixElements(mat, matelem);

    // This final model matrix is passed to the shader program as the model-view-projection matrix.
    oriSetUniformMat4x4f(shader, "transform.mvp", false, &matelem[0][0]);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // The vertex array, index buffer, and shader are bound.
        oriBindVertexArray(vao);
        oriBindBuffer(ibo, GL_ELEMENT_ARRAY_BUFFER); // (note that 'ibo' is bound to GL_ELEMENT_ARRAY_BUFFER so OpenGL uses it as an index buffer)
        oriBindShader(shader);

        // The final square is rendered to the window.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        // More GLFW functions to swap window buffers (double-buffering) and poll window events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // The previously used matrix and vector are freed.
    zmlFreeMatrix(&mat);
    zmlFreeVector(&vec);

    // Orion is terminated (BEFORE terminating GLFW)
    oriTerminate();
    glfwTerminate(); // Note that Orion does not automatically terminate GLFW in this case - the graphics library is in fact unaware of GLFW's usage in the first place.

    return 0;
}
