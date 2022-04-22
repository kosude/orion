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

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// unsigned int vboold;
	// glCreateBuffers(1, &vboold);
	// glBindBuffer(GL_ARRAY_BUFFER, vboold);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	oriBuffer *vbo = oriCreateBuffer(GL_ARRAY_BUFFER);
	oriSetBufferData(vbo, vertices, sizeof(vertices), GL_STATIC_DRAW);
	oriBindBuffer(vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, 0, 7 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 4, GL_FLOAT, 0, 7 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	oriSetUniform1i(shader, "blend.mode", 3);

	zmlMatrix model = zmlIdentityMatrix(4, 4);

	while (!oriWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		zmlRotate(&model, zmlToRadians(1.0f), 0, 0, 1);

		float elements[4][4];
		zmlCopyMatrixElements(model, elements);
		oriSetUniformMat4x4f(shader, "transform.mvp", 0, &elements[0][0]);

		oriBindShader(shader);
		// oriBindBuffer(vbo);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		oriPollEvents();
		oriSwapBuffers(window);
	}

	zmlFreeMatrix(&model);

	oriTerminate();
	return 0;
}