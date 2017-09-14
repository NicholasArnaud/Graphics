#pragma once
#include "Application.h"
#include "Mesh.h"
#include "Shader.h"

class Camera;

class GeometryApplication : public Application
{
public:
	GeometryApplication();
	~GeometryApplication();
	unsigned int m_programID;
	Camera* camera;
	Mesh* mesh;
	Shader* shader;

	void GenObject(int select, int numPoints, int nummedians);
protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
};

