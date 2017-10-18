#pragma once
#include "Application.h"
#include <glm.hpp>


struct Mesh;
struct Shader;
struct Camera;
class ProceduralGenerationApp :
	public Application
{
public:
	ProceduralGenerationApp();
	~ProceduralGenerationApp();
	Shader* shader;
	Mesh* mesh;
	Camera* cam;

	unsigned int perlin_data;

protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	float interpolate(float a, float b, float x);
	double noise(int x, int y);
	float* genNoiseTex(unsigned width, unsigned height);
	Mesh* generateGrid(unsigned rows, unsigned cols);
	double nick_perlin(glm::vec2 pos);
};

