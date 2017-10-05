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
	unsigned int perlin_data;
	unsigned texture4;


protected:
	Mesh* generateGrid(unsigned rows, unsigned cols);
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	void genTexPlane() const;
	static void setupTexture(char * imageLoc, unsigned int * texture);
	static void drawTex(Shader* shader, unsigned* texture, int numOfTextures);
	float* genNoiseTex(unsigned width, unsigned height);
	void generateSphere(const unsigned segments, const unsigned rings, unsigned& vao, unsigned& vbo, unsigned& ibo,
	                    unsigned& indexCount) const;

	Shader* crateShader;
	Shader* shader;
	Mesh* planeMesh;
	Mesh* sphereMesh;
	Camera* cam;
};

