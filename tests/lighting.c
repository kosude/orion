#define ORION_TK_DEBUG_CONTEXT false
#include "testkit/oriontk.h"

#include <zetaml/include/zetaml.h>
#include <stb_image/stb_image.h>

oriBuffer *vbo, *ibo;
oriVertexArray *vao;
oriShader *shader;
oriTexture *orangebox;

float cubeRot = 0;

// ======================================================================================
// *****                                  PRELOAD()                                 *****
// ======================================================================================

void preload() {
    oritk.windowWidth = 640;
    oritk.windowHeight = 480;
    oritk.windowTitle = "Orion lighting test";

    oritk.glVersion = 330;
    oritk.glProfile = GLFW_OPENGL_CORE_PROFILE;
}

// ======================================================================================
// *****                                INITIALISE()                                *****
// ======================================================================================

void initialise() {
    vbo = oriCreateBuffer();
    oriSetBufferData(vbo, cubeVertices, sizeof(cubeVertices), GL_STATIC_DRAW);

    vao = oriCreateVertexArray();
    oriSpecifyVertexData(vao, vbo, 0, 3, GL_FLOAT, false, 8 * sizeof(float), 0); // vertex positions
    oriSpecifyVertexData(vao, vbo, 1, 2, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float)); // tex coords

    shader = oriCreateShader();
    oriAddShaderSource(shader, GL_VERTEX_SHADER, ORION_VERTEX_SHADER_BASIC);
    oriAddShaderSource(shader, GL_FRAGMENT_SHADER, ORION_FRAGMENT_SHADER_BASIC);
    oriSetUniform1i(shader, "blend.mode", 2);

    orangebox = oriCreateTexture(GL_TEXTURE_2D, GL_RGBA);
    stbi_set_flip_vertically_on_load(1);
    int x, y, d;
    unsigned char *image = stbi_load("resources/orangebox.png", &x, &y, &d, 4);
    oriUploadTexImage(orangebox, GL_UNSIGNED_BYTE, image, x, y, 0, GL_RGBA);
    stbi_image_free(image);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
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

    cubeRot += 0.5 * oritk.windowDeltaTime;

    // Model matrix
    zmlMatrix model = zmlRotateIdentity(cubeRot, 0, 1, 0);

    // View matrix
    zmlVector pos = zmlConstructVectorDefault(3, 1.2);
    pos.elements[0] = 0;
    zmlVector focus = zmlConstructVectorDefault(3, 0);
    zmlVector up = zmlConstructVectorDefault(3, 0);
    up.elements[1] = 1;
    zmlMatrix view = zmlConstructLookAtMatrixRH(pos, focus, up);

    // Projection matrix
    zmlMatrix proj = zmlConstructPerspectiveMatrixRH(0.1f, 1000.0f, zmlToRadians(45.0f), (double) w / (double) h);
    // MVP matrix
    zmlMatrix mvp = zmlCopyMatrix(&proj);
    zmlMultiplyMats(&mvp, view);
    zmlMultiplyMats(&mvp, model);
    float matelem[4][4];
    zmlCopyMatrixElements(mvp, matelem);
    oriSetUniformMat4x4f(shader, "transform.mvp", true, &matelem[0][0]);

    // =============================
    //      RENDER
    // =============================

	glClearColor(0.08, 0.12, 0.22, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    oriBindTexture(orangebox, 0);
    oriBindVertexArray(vao);
    oriBindShader(shader);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    oriSwapBuffers(oritk.window);
    oriPollEvents();

    // =============================
    //      FREE
    // =============================

    zmlFreeVector(&pos);
    zmlFreeVector(&focus);
    zmlFreeVector(&up);
    zmlFreeMatrix(&model);
    zmlFreeMatrix(&view);
    zmlFreeMatrix(&proj);
    zmlFreeMatrix(&mvp);
}

// ======================================================================================
// *****                                   CLEAN()                                  *****
// ======================================================================================

void clean() {}
