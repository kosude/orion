#include <oriongl.h>
#include <orionwin.h>
#include <stdlib.h>

float vertices[] = {
    -0.5f, -0.5f, 0.0f, 1, 0, 0, 1,
     0.5f, -0.5f, 0.0f, 1, 1, 0, 1,
     0.0f,  0.5f, 0.0f, 0, 1, 1, 1
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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

	while (!oriWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		oriBindShader(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		oriPollEvents();
		oriSwapBuffers(window);
	}

	oriTerminate();
	return 0;
}