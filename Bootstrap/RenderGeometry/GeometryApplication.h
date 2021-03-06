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
	std::vector<unsigned> GenIndices(int numP, int numM);
	std::vector<Vertex> GenHalfCircle(float rad, int np);
	std::vector<Vertex> GenSphere(std::vector<Vertex> points, int nm, int rad);
	void generateSphere(unsigned int segments, unsigned int rings,
		unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
		unsigned int& indexCount);
protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;

};

