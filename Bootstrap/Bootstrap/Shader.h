#pragma once
#include "Filer.h"

class Shader
{
public:
	Shader();
	~Shader();
	void bind();
	void unbind();
	void load(const char * filename, unsigned int type);
	void attach();
	void defaultLoad();

	unsigned int program();
	unsigned int getUniform(const char *);
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;

	const char* vsSource;
	const char* fsSource;


private:
	unsigned int m_program;
	Filer* file;
};
