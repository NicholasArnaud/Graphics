#pragma once
#include <vector>
#include <ext.hpp>

struct Vertex
{
	glm::vec4 position;
	glm::vec4 colour;
	glm::vec4 normal;
	glm::vec4 tangent;
	glm::vec4 bitangent;
	glm::vec2 texcoord;
};
class Mesh
{
public:
	Mesh();
	~Mesh();
	void create_buffers();
	void initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices);
	void bind();
	void unbind();

	unsigned int vertex_count;
	unsigned int index_count;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
};

