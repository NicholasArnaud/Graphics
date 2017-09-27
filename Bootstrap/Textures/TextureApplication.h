#pragma once
#include <Application.h>
#include <Shader.h>
#include <Mesh.h>
#include <Camera.h>

class TextureApplication :
	public Application
{
public:
	TextureApplication();
	~TextureApplication();

	unsigned int texture;

protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	void genTexPlane(unsigned& vao, unsigned& vbo, unsigned& ibo);
	Shader* shader;
	Mesh* planeMesh;
	Camera* cam;
};

