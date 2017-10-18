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

	unsigned int texture1;
	unsigned int texture2;
	unsigned int texture3;
	unsigned texture4;
	


protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	static void setupTexture(char * imageLoc, unsigned int * texture);
	static void drawTex(Shader* shader, unsigned* texture, int numOfTextures);
	void generateSphere(const unsigned segments, const unsigned rings, unsigned& vao, unsigned& vbo, unsigned& ibo,
	                    unsigned& indexCount) const;

	Shader* planeShader;
	Shader* shader;
	Mesh* planeMesh;
	Mesh* sphereMesh;
	Camera* cam;
};

