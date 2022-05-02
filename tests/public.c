#include "oriongl.h"
#include "orionwin.h"

#include <zetaml/include/zetaml.h>
#include <stb_image/stb_image.h>

float vertices[] = {
    -0.5f, -0.5f, 0.0f,     1, 0, 0, 1,     0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,     0, 1, 0, 1,     1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,     0, 0, 1, 1,     0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,     1, 1, 0, 1,     1.0f, 1.0f
};

unsigned int indices[] = {
    0, 1, 2,
    3, 1, 2
};

int main() {
    oriWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    oriWindow *mainWin = oriCreateWindow(480, 480, "Orion public interface test", 430, GLFW_OPENGL_CORE_PROFILE);
    oriSwapInterval(mainWin, 1);

    oriInitialise(430);

    oriSetFlag(ORION_DEBUG_CONTEXT, true);
    oriDebugFlags(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, GL_DONT_CARE, false, NULL, 0);

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
    oriSetUniform1i(shader, "blend.mode", 6);

    oriTexture *texture = oriCreateTexture(GL_TEXTURE_2D, GL_RGBA);
    stbi_set_flip_vertically_on_load(1);
    int x, y, d;
    unsigned char *image = stbi_load("resources/onions.jpg", &x, &y, &d, 4);
    oriUploadTexImage(texture, GL_UNSIGNED_BYTE, image, x, y, 0, GL_RGBA);

    stbi_image_free(image);

    while (!oriWindowShouldClose(mainWin)) {
        unsigned int w, h;
        oriGetWindowSize(mainWin, &w, &h);
        glViewport(0, 0, w, h);

        zmlVector scale = zmlConstructVectorDefault(3, 1.41f);
        zmlVector trans = zmlConstructVectorDefault(3, 0);
        trans.elements[2] = -0.5f;
        zmlMatrix model = zmlIdentityMatrix(4, 4);
        zmlScale(&model, scale);
        zmlTranslate(&model, trans); 

        zmlMatrix proj = zmlConstructPerspectiveMatrixRH(0.0f, 1000.0f, zmlToRadians(45.0f), (double) w / (double) h);

        zmlMatrix mat = zmlMultiplyMats_r(proj, model);
        float matelem[4][4];
        zmlCopyMatrixElements(mat, matelem);
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
        zmlFreeVector(&trans);
        zmlFreeMatrix(&model);
        zmlFreeMatrix(&proj);
        zmlFreeMatrix(&mat);
    }

    oriTerminate();
    return 0;
}
