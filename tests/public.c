#include "oriongl.h"
#include "orionwin.h"

#include <zetaml/include/zetaml.h>

float vertices[] = {
    -0.5f, -0.5f, 0.5f,     1, 0, 0, 1,     0.0f, 0.0f,
     0.5f, -0.5f, 0.5f,     0, 1, 0, 1,     1.0f, 0.0f,
    -0.5f,  0.5f, 0.5f,     0, 0, 1, 1,     0.0f, 1.0f,
     0.5f,  0.5f, 0.5f,     1, 1, 0, 1,     1.0f, 1.0f
};

unsigned int indices[] = {
    0, 1, 2,
    3, 1, 2
};

int main() {
    oriWindow *mainWin = oriCreateWindow(480, 480, "Orion public interface test", 330, GLFW_OPENGL_CORE_PROFILE);
    oriSwapInterval(mainWin, 1);

    oriInitialise(450);

    oriBuffer *ibo = oriCreateBuffer();
    oriSetBufferData(ibo, indices, sizeof(indices), GL_STATIC_DRAW);

    oriBuffer *vbo = oriCreateBuffer();
    oriSetBufferData(vbo, vertices, sizeof(vertices), GL_STATIC_DRAW);
    oriVertexArray *vao = oriCreateVertexArray();
    oriSpecifyVertexData(vao, vbo, 0, 3, GL_FLOAT, false, 9 * sizeof(float), 0 * sizeof(float)); // vertex positions
    oriSpecifyVertexData(vao, vbo, 2, 4, GL_FLOAT, false, 9 * sizeof(float), 3 * sizeof(float)); // vertex colours
    oriSpecifyVertexData(vao, vbo, 1, 2, GL_FLOAT, false, 9 * sizeof(float), 7 * sizeof(float)); // tex coords

    oriShader *shader = oriCreateShader();
    oriAddShaderSource(shader, GL_VERTEX_SHADER, ORION_VERTEX_SHADER_BASIC);
    oriAddShaderSource(shader, GL_FRAGMENT_SHADER, ORION_FRAGMENT_SHADER_BASIC);
    oriSetUniform1i(shader, "blend.mode", 2);

    oriTexture *texture = oriCreateTexture(GL_TEXTURE_2D, GL_RGB);
    oriSetTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    oriSetTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    oriSetTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    oriSetTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    oriUploadTexImagePath(texture, "resources/testimage.jpg", 3, GL_RGB);

    while (!oriWindowShouldClose(mainWin)) {
        unsigned int w, h;
        oriGetWindowSize(mainWin, &w, &h);
        glViewport(0, 0, w, h);

        zmlVector scale = zmlConstructVectorDefault(3, 1.5f);
        zmlMatrix model = zmlIdentityMatrix(4, 4);
        zmlScale(&model, scale);

        float matelem[4][4];
        zmlCopyMatrixElements(model, matelem);
        oriSetUniformMat4x4f(shader, "transform.mvp", false, &matelem[0][0]);

        glClear(GL_COLOR_BUFFER_BIT);

        oriBindTexture(texture, 0);
        oriBindVertexArray(vao);
        oriBindBuffer(ibo, GL_ELEMENT_ARRAY_BUFFER);
        oriBindShader(shader);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

        oriSwapBuffers(mainWin);
        oriPollEvents();

        zmlFreeVector(&scale);
        zmlFreeMatrix(&model);
    }

    oriTerminate();
    return 0;
}
