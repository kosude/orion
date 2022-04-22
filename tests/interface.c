#include "oriongl.h"
#include "orionwin.h"

#include <zetaml.h>

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1, 0, 0, 1,
     0.5f, -0.5f, 0.0f, 0, 1, 0, 1,
     0.0f,  0.5f, 0.0f, 0, 0, 1, 1
};

int main() {
	oriWindow *window = oriCreateWindow(640, 480, "Orion GLFW test", 460, GLFW_OPENGL_COMPAT_PROFILE);

	oriInitialise(460);
	oriEnableDebugContext(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, GL_DONT_CARE, false, NULL, 0);

	oriShader *shader = oriCreateShader();
	oriAddShaderSource(shader, GL_VERTEX_SHADER, ORION_VERTEX_SHADER_BASIC);
	oriAddShaderSource(shader, GL_FRAGMENT_SHADER, ORION_FRAGMENT_SHADER_BASIC);
	oriSetUniform1i(shader, "blend.mode", 3);

	oriBuffer *vbo = oriCreateBuffer(GL_ARRAY_BUFFER);
	oriSetBufferData(vbo, vertices, sizeof(vertices), GL_STATIC_DRAW);

	oriVertexArray *vao = oriCreateVertexArray();
	oriSpecifyVertexData(vao, vbo, 0, 3, GL_FLOAT, false, 7 * sizeof(float), 0);
	oriSpecifyVertexData(vao, vbo, 2, 4, GL_FLOAT, false, 7 * sizeof(float), 3 * sizeof(float));

	zmlMatrix model = zmlIdentityMatrix(4, 4);

	while (!oriWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		zmlRotate(&model, zmlToRadians(1.0f), 0, 0, 1);

		float elements[4][4];
		zmlCopyMatrixElements(model, elements);
		oriSetUniformMat4x4f(shader, "transform.mvp", 0, &elements[0][0]);

		oriBindShader(shader);
		oriBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		oriPollEvents();
		oriSwapBuffers(window);
	}

	zmlFreeMatrix(&model);

	oriTerminate();
	return 0;
}