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
	Camera* camera;

	/**
	 * \brief mesh for unsual shapes or shapes that don't have a specific mesh already created
	 */
	Mesh* generalMesh;
	Mesh* sphereMesh;
	Mesh* triangleMesh;
	Mesh* cubeMesh;
	Mesh* planeMesh;
	
	Shader* shader;

	void drawMesh(unsigned drawfill, unsigned drawstyle, Mesh* mesh);
	void GenObject(Mesh* mesh, int select, int numP, int numM);
	void generateSphere(unsigned segments, unsigned rings, unsigned& vao, unsigned& vbo, unsigned& ibo, unsigned& indexCount);
protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;

};

