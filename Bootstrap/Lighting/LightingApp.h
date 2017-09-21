#pragma once
#include "Application.h"
#include "GeometryApplication.h"

class LightingApp :
	public Application
{
public:
	LightingApp();
	~LightingApp();

	Mesh* sphereMesh;
	Mesh* gridMesh;
	Shader* shader;
	Camera* cam;
	bool m_Quit;
protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	void GenGrid();
	void drawMesh(unsigned drawfill, unsigned drawstyle, Mesh* sphereMesh);
	void generateSphere(unsigned segments, unsigned rings, unsigned& vao, unsigned& vbo, unsigned& ibo, unsigned& indexCount);
};

