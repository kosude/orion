#include "testkit/oriontk.h"

#include <zetaml/include/zetaml.h>
#include <stb_image/stb_image.h>

oriBuffer *ibo, *vbo;
oriVertexArray *vao;
oriShader *shader;
oriTexture *onions;

// ======================================================================================
// *****                                  PRELOAD()                                 *****
// ======================================================================================

void preload() {
    oritk.windowWidth = 480;
    oritk.windowHeight = 480;
    oritk.windowTitle = "Orion public interface test";

    oritk.glVersion = 430;
    oritk.glProfile = GLFW_OPENGL_CORE_PROFILE;

    oriWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
}

// ======================================================================================
// *****                                INITIALISE()                                *****
// ======================================================================================

void initialise() {
    ibo = oriCreateBuffer();
    oriSetBufferData(ibo, squareIndices, sizeof(squareIndices), GL_STATIC_DRAW);

    vbo = oriCreateBuffer();
    oriSetBufferData(vbo, squareVertices, sizeof(squareVertices), GL_STATIC_DRAW);

    vao = oriCreateVertexArray();
    oriSpecifyVertexData(vao, vbo, 0, 3, GL_FLOAT, false, 9 * sizeof(float), 0 * sizeof(float)); // vertex positions
    oriSpecifyVertexData(vao, vbo, 2, 4, GL_FLOAT, false, 9 * sizeof(float), 3 * sizeof(float)); // vertex colours
    oriSpecifyVertexData(vao, vbo, 1, 2, GL_FLOAT, false, 9 * sizeof(float), 7 * sizeof(float)); // tex coords

    shader = oriCreateShader();
    oriAddShaderSource(shader, GL_VERTEX_SHADER, ORION_VERTEX_SHADER_BASIC);
    oriAddShaderSource(shader, GL_FRAGMENT_SHADER, ORION_FRAGMENT_SHADER_BASIC);
    oriSetUniform1i(shader, "blend.mode", 6);

    onions = oriCreateTexture(GL_TEXTURE_2D, GL_RGBA);
    stbi_set_flip_vertically_on_load(1);
    int x, y, d;
    unsigned char *image = stbi_load("resources/onions.jpg", &x, &y, &d, 4);
    oriUploadTexImage(onions, GL_UNSIGNED_BYTE, image, x, y, 0, GL_RGBA);
    stbi_image_free(image);
}

// ======================================================================================
// *****                                   RENDER()                                 *****
// ======================================================================================

void render() {
    // =============================
    //      UPDATE
    // =============================

    unsigned int w, h;
    oriGetWindowSize(oritk.window, &w, &h);
    glViewport(0, 0, w, h);

    // Model matrix
    zmlVector scale = zmlConstructVectorDefault(3, 1.41f);
    zmlVector trans = zmlConstructVectorDefault(3, 0);
    trans.elements[2] = -0.5f;
    zmlMatrix model = zmlIdentityMatrix(4, 4);
    zmlScale(&model, scale);
    zmlTranslate(&model, trans); 

    // Projection matrix
    zmlMatrix proj = zmlConstructPerspectiveMatrixRH(0.0f, 1000.0f, zmlToRadians(45.0f), (double) w / (double) h);

    // MVP matrix
    zmlMatrix mat = zmlMultiplyMats_r(proj, model);
    float matelem[4][4];
    zmlCopyMatrixElements(mat, matelem);
    oriSetUniformMat4x4f(shader, "transform.mvp", false, &matelem[0][0]);

    // =============================
    //      RENDER
    // =============================

    glClear(GL_COLOR_BUFFER_BIT);

    oriBindTexture(onions, 0);
    oriBindVertexArray(vao);
    oriBindBuffer(ibo, GL_ELEMENT_ARRAY_BUFFER);
    oriBindShader(shader);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    oriSwapBuffers(oritk.window);
    oriPollEvents();

    // =============================
    //      FREE
    // =============================

    zmlFreeVector(&scale);
    zmlFreeVector(&trans);
    zmlFreeMatrix(&model);
    zmlFreeMatrix(&proj);
    zmlFreeMatrix(&mat);
}

// ======================================================================================
// *****                                   CLEAN()                                  *****
// ======================================================================================

void clean() {}
