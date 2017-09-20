#pragma once
#include "Application.h"
#include "GeometryApplication.h"

class LightingApp :
	public Application
{
public:
	LightingApp();
	~LightingApp();

	Mesh* mesh;
	Shader* shader;
	Camera* cam;
	GeometryApplication* geoapp;
protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	void GenGrid();
};

