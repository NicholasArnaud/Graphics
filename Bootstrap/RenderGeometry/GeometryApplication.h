#pragma once
#include "Application.h"
#include "Mesh.h"

class Camera;

class GeometryApplication : public Application
{
public:
	GeometryApplication();
	~GeometryApplication();
	unsigned int m_programID;
	Camera* camera;
	Mesh* mesh;
	
	void GenObject(int numb);
	void run(const char* title, unsigned width, unsigned height, bool fullscreen) override;
protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
};

