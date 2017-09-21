#pragma once
#include "Application.h"
#include "GeometryApplication.h"


struct DirectionalLight
{
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 direction;
};
struct Material
{
	glm::vec3 diffuse;
	glm::vec3 ambient;
	glm::vec3 specular;
	int specularPower;
};
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
	glm::vec3 m_ambientLight;
	Material m_material;
	DirectionalLight m_directionalLight;
	glm::mat4 m_modelMatrix;
protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	void drawMesh(unsigned drawfill, unsigned drawstyle, Mesh* sphereMesh);
	void generateSphere(unsigned segments, unsigned rings, unsigned& vao, unsigned& vbo, unsigned& ibo, unsigned& indexCount);
};

