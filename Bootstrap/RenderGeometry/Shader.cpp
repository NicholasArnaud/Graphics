#include "Shader.h"
#include "gl_core_4_4.h"
#include "Filer.h"
#include <cstdio>


Shader::Shader() : m_vertexShader(0), m_fragmentShader(0), vsSource(nullptr), fsSource(nullptr), m_program(0), file(nullptr)
{
	file = new Filer();
}


Shader::~Shader()
{
}

void Shader::bind()
{
	glUseProgram(m_program);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::load(const char * filename, unsigned int type)
{
	const char* contents = file->readIt(filename);
	switch (type)
	{
	case GL_VERTEX_SHADER:
		
		m_vertexShader = glCreateShader(type);
		glShaderSource(m_vertexShader, 1, (const char **)&contents, 0);
		glCompileShader(m_vertexShader);
		break;

	case GL_FRAGMENT_SHADER:
		
		m_fragmentShader = glCreateShader(type);
		glShaderSource(m_fragmentShader, 1, (const char **)&contents, 0);
		glCompileShader(m_fragmentShader);
		break;
	default:
		break;
	}
	m_program = glCreateProgram();
	this->attach();
	glLinkProgram(m_program);
	int success = GL_FALSE;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) 
	{
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
}

void Shader::attach()
{
	glAttachShader(m_program, m_vertexShader);
	glAttachShader(m_program, m_fragmentShader);
}

void Shader::defaultLoad()
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
	file->writeIt("Vertex.vert", vsSource);
	file->writeIt("Fragment.frag", fsSource);

	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//specify where the source is located and bind to the specific shader handle
	glShaderSource(m_vertexShader, 1, (const char **)&vsSource, 0);
	glCompileShader(m_vertexShader);
	glShaderSource(m_fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(m_fragmentShader);
	m_program = glCreateProgram();
	this->attach();
	glLinkProgram(m_program);
	int success = GL_FALSE;
	// check that it compiled and linked correctly
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength + 1];
		glGetProgramInfoLog(m_program, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
}

unsigned Shader::program()
{
	return m_program;
}

unsigned int Shader::getUniform(const char *)
{
	return 0;
}
