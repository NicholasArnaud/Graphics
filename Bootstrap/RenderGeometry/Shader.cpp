#include "Shader.h"
#include "gl_core_4_4.h"
#include <cstdio>


Shader::Shader() : m_vertexShader(0), m_fragmentShader(0), vsSource(nullptr), fsSource(nullptr), m_program(0)
{
}


Shader::~Shader()
{
}

void Shader::bind()
{
	glLinkProgram(m_program);
}

void Shader::unbind()
{
	glLinkProgram(0);
}

void Shader::load(const char * filename, unsigned int type)
{
	FILE* fp;
	switch (type)
	{
	case GL_FRAGMENT_SHADER:
		fsSource = "#version 410\n \
	in vec4 vColour; \
	out vec4 fragColour;\
	void main() { fragColour = vColour; }";
		fopen_s(&fp, filename, "w");
		fputs(fsSource, fp);
		fclose(fp);
		break;
	case GL_VERTEX_SHADER:
		vsSource = "#version 410\n \
	layout(location=0) in vec4 position; \
	layout(location=1) in vec4 colour; \
	out vec4 vColour; \
	uniform mat4 projectionViewWorldMatrix; \
	void main() { vColour = colour; gl_Position =\
	projectionViewWorldMatrix * position; }";

		fopen_s(&fp, filename, "w");
		fputs(vsSource, fp);
		fclose(fp);
		break;


	default:
		break;
	}
}

void Shader::attach()
{
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
}

void Shader::defaultLoad()
{
	load("VertexShader.vert", GL_VERTEX_SHADER);
	load("FragmentShader.frag", GL_FRAGMENT_SHADER);
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(m_vertexShader);
	glShaderSource(m_fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(m_fragmentShader);

	m_program = glCreateProgram();
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);

	bind();

	int success = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_program, infoLogLength, nullptr, infoLog);
		printf("ERROR: Failed to link shader program! \n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
}

unsigned int Shader::getUniform(const char *)
{
	return 0;
}
