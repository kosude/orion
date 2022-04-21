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

	unsigned int vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(ORION_VERTEX_POS, 3, GL_FLOAT, 0, 7 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(ORION_VERTEX_POS);
	glVertexAttribPointer(ORION_VERTEX_COLOUR, 4, GL_FLOAT, 0, 7 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(ORION_VERTEX_COLOUR);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	oriSetUniform1i(shader, "blend.mode", 3);

	zmlMatrix identity = zmlIdentityMatrix(4, 4);
	float elements[4][4];
	zmlCopyMatrixElements(identity, elements);

	oriSetUniformMat4x4f(shader, "transform.mvp", 0, &elements[0][0]);

	while (!oriWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		oriBindShader(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		oriPollEvents();
		oriSwapBuffers(window);
	}

	zmlFreeMatrix(&identity);

	oriTerminate();
	return 0;
}