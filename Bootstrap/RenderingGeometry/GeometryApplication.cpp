#include "GeometryApplication.h"
#include "CameraApp.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <cstdio>


struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;
};

GeometryApplication::GeometryApplication() : m_VAO(0), m_VBO(0), m_IBO(0), m_programID(0), m_rows(0), m_cols(0), camera(nullptr)
{
}

GeometryApplication::~GeometryApplication()
{
}

void GeometryApplication::startup()
{
	const char* vsSource = "#version 410\n \
	layout(location=0) in vec4 position; \
	layout(location=1) in vec4 colour; \
	out vec4 vColour; \
	uniform mat4 projectionViewWorldMatrix; \
	void main() { vColour = colour; gl_Position =\
	projectionViewWorldMatrix * position; }";
	const char* fsSource = "#version 410\n \
	in vec4 vColour; \
	out vec4 fragColour;\
	void main() { fragColour = vColour; }";
	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("ERROR: Failed to link shader program! \n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	generateGrid(20, 20);
}

void GeometryApplication::shutdown()
{
}

void GeometryApplication::update(float)
{
}

void GeometryApplication::draw()
{
}

void GeometryApplication::generateGrid(unsigned int rows, unsigned int cols)
{
	m_rows = rows;
	m_cols = cols;
	Vertex* aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; r++)
	{
		for (unsigned int c = 0; c < cols; c++)
		{
			aoVertices[r * cols + c].position = glm::vec4((float)c, 0, (float)r, 1);

			glm::vec3 colour = glm::vec3(sinf(c / (float)(cols - 1)* r / (float)(rows - 1)));
			aoVertices[r* cols + c].colour = glm::vec4(colour, 1);
		}
	}

	unsigned int* auiIndices = new unsigned int[(rows - 1)* (cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
		{
			auiIndices[index++] = r* cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1)* cols + (c + 1);

			auiIndices[index++] = r* cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r* cols + (c + 1);
		}
	}

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex), aoVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 * sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(m_programID);
	unsigned int projectionViewTransform = 
		glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	//glUniform4fv(projectionViewTransform, 1, );
	glBindVertexArray(m_VAO);
	unsigned int indexCount = (rows - 1)* (cols - 1 * 6);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	delete[] auiIndices;
	delete[] aoVertices;
}

void GeometryApplication::run(const char * title, unsigned width, unsigned height, bool fullscreen)
{
	glfwInit();
	GLFWwindow* m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	
	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return;
	}

	startup();
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glEnable(GL_DEPTH_TEST);
	while (!m_gameover)
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		draw();

		glfwSwapBuffers(m_window);
		m_gameover = (glfwWindowShouldClose(m_window) == GLFW_TRUE);
	}
	

}
