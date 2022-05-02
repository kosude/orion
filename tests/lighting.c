#define ORION_TK_DEBUG_CONTEXT false
#include "testkit/oriontk.h"

#include <zetaml/include/zetaml.h>
#include <stb_image/stb_image.h>

oriBuffer *vbo, *ibo;
oriVertexArray *vao;
oriShader *shader;
oriTexture *box, *box_s;

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
    oriSpecifyVertexData(vao, vbo, 3, 3, GL_FLOAT, false, 8 * sizeof(float), 5 * sizeof(float)); // normals

    shader = oriCreateShader();
    oriAddShaderSource(shader, GL_VERTEX_SHADER, ORION_VERTEX_SHADER_LIGHTING);
    oriAddShaderSource(shader, GL_FRAGMENT_SHADER, ORION_FRAGMENT_SHADER_LIGHTING);
    
    oriSetUniform1i(shader, "material.tex", 0);
    oriSetUniform1i(shader, "material.specularTex", 1);
    oriSetUniform3f(shader, "material.colour", 1, 1, 1 );
    oriSetUniform1i(shader, "material.shininess", 2);

    oriSetUniform3f(shader, "lightSource.colour", 1, 1, 1);
    oriSetUniform3f(shader, "lightSource.pos", 0.0f, 1.5f, 1.2f);
    oriSetUniform1f(shader, "lightSource.brightness", 1);
    oriSetUniform1f(shader, "lightSource.ambientIntensity", 0.4f);
    oriSetUniform1f(shader, "lightSource.diffuseIntensity", 1.0f);
    oriSetUniform1f(shader, "lightSource.specularIntensity", 1.5f);

    oriSetUniform3f(shader, "view.pos", 0.0f, 1.2f, 1.2f);

    box = oriCreateTexture(GL_TEXTURE_2D, GL_RGBA);
    stbi_set_flip_vertically_on_load(1);
    int x, y, d;
    unsigned char *image = stbi_load("resources/container.png", &x, &y, &d, 4);
    oriUploadTexImage(box, GL_UNSIGNED_BYTE, image, x, y, 0, GL_RGBA);
    stbi_image_free(image);

    box_s = oriCreateTexture(GL_TEXTURE_2D, GL_RGBA);
    stbi_set_flip_vertically_on_load(1);
    image = stbi_load("resources/container_specular.png", &x, &y, &d, 4);
    oriUploadTexImage(box_s, GL_UNSIGNED_BYTE, image, x, y, 0, GL_RGBA);
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

    cubeRot += 1.0 * oritk.windowDeltaTime;

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

    float matelem_model[4][4];
    zmlCopyMatrixElements(model, matelem_model);
    oriSetUniformMat4x4f(shader, "transform.model", true, &matelem_model[0][0]);
    float matelem_projview[4][4];
    zmlCopyMatrixElements(mvp, matelem_projview);
    oriSetUniformMat4x4f(shader, "transform.projView", true, &matelem_projview[0][0]);

    // =============================
    //      RENDER
    // =============================

	glClearColor(0.08, 0.12, 0.22, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    oriBindTexture(box, 0);
    oriBindTexture(box_s, 1);
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
