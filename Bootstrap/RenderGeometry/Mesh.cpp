#include "Mesh.h"
#include "gl_core_4_4.h"


Mesh::Mesh(): vertex_count(0), index_count(0), m_VAO(0), m_VBO(0), m_IBO(0)
{
}

Mesh::~Mesh()
{
}

void Mesh::create_buffers()
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(glm::vec4));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (sizeof(glm::vec4)*2));

}

void Mesh::initialize(std::vector<Vertex>& verts, std::vector<unsigned int>& indices)
{
	for(auto v: verts)
	{
		m_vertices.push_back(v);
	}
	for(auto i: indices)
	{
		m_indices.push_back(i);
	}

	index_count = m_indices.size();
	vertex_count = m_vertices.size();
}

void Mesh::bind()
{
	glBindVertexArray(m_VAO);
}

void Mesh::unbind()
{
	glBindVertexArray(0);
}
