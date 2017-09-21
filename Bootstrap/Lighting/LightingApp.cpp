#include "LightingApp.h"
#include "Camera.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#define PI 3.141592653


LightingApp::LightingApp() : sphereMesh(nullptr), gridMesh(nullptr), shader(nullptr), m_Quit(false)
{
	sphereMesh = new Mesh();
	gridMesh = new Mesh();
	shader = new Shader();
	cam = new Camera();
	cam->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 1.0f, 1000.f);
}


LightingApp::~LightingApp()
{
}

void LightingApp::startup()
{
	
	cam->setLookAt(glm::vec3(-10, 10, 10), glm::vec3(1), glm::vec3(0, 1, 0));
	shader->load("LightingVertex.vert", GL_VERTEX_SHADER);
	shader->load("LightingFragment.frag", GL_FRAGMENT_SHADER);

	GenGrid();
	generateSphere(15, 15, sphereMesh->m_VAO, sphereMesh->m_VBO, sphereMesh->m_IBO, sphereMesh->index_count);
}

void LightingApp::shutdown()
{
}


void LightingApp::update(float)
{
	glfwPollEvents();
	if (glfwGetKey(m_window, GLFW_KEY_W))
		cam->setPosition(cam->getWorldTransform()[3] -= cam->getWorldTransform()[2] * .2f);
	if (glfwGetKey(m_window, GLFW_KEY_S))
		cam->setPosition(cam->getWorldTransform()[3] += cam->getWorldTransform()[2] * .2f);
	if (glfwGetKey(m_window, GLFW_KEY_A))
		cam->setPosition(cam->getWorldTransform()[3] -= cam->getWorldTransform()[0] * .2f);
	if (glfwGetKey(m_window, GLFW_KEY_D))
		cam->setPosition(cam->getWorldTransform()[3] += cam->getWorldTransform()[0] * .2f);

	static bool sbMouseButtonDown = false;
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {

		static double siPrevMouseX = 0;
		static double siPrevMouseY = 0;

		if (sbMouseButtonDown == false)
		{
			sbMouseButtonDown = true;
			glfwGetCursorPos(m_window, &siPrevMouseX, &siPrevMouseY);
		}

		double mouseX = 0, mouseY = 0;
		glfwGetCursorPos(m_window, &mouseX, &mouseY);

		double iDeltaX = mouseX - siPrevMouseX;
		double iDeltaY = mouseY - siPrevMouseY;

		siPrevMouseX = mouseX;
		siPrevMouseY = mouseY;

		glm::mat4 rEle = rotate((float)iDeltaX * 1 / 800, glm::vec3(0, 1, 0));
		glm::mat4 rAzi = rotate((float)iDeltaY * 1 / 800, glm::vec3(1, 0, 0));
		cam->m_worldTransform = rEle* rAzi * cam->m_worldTransform;
		cam->m_viewTransform = glm::inverse(cam->m_worldTransform);
	}
}


void LightingApp::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	drawMesh(GL_LINE, GL_TRIANGLES, gridMesh);
	drawMesh(GL_FILL, GL_TRIANGLE_STRIP, sphereMesh);
}

void LightingApp::GenGrid()
{
	Vertex * aoVertices = new Vertex[100];
	for (unsigned int r = 0; r < 10; ++r)
		for (unsigned int c = 0; c < 10; ++c)
			aoVertices[r * 10 + c].position = glm::vec4((float)c, 0, (float)r, 1);
	const unsigned int numitems = (10 - 1) * (10 - 1) * 6;
	auto auiIndices = new unsigned int[numitems];

	unsigned int index = 0;
	for (unsigned int r = 0; r < 10 - 1; ++r)
		for (unsigned int c = 0; c < 10 - 1; ++c)
		{
			//Triangle 1
			auiIndices[index++] = r * 10 + c;
			auiIndices[index++] = (r + 1) * 10 + c;
			auiIndices[index++] = (r + 1) * 10 + (c + 1);
			//Triangle 2
			auiIndices[index++] = r * 10 + c;
			auiIndices[index++] = (r + 1) * 10 + (c + 1);
			auiIndices[index++] = r * 10 + (c + 1);
		}

	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < 10 * 10; i++)
		verts.push_back(aoVertices[i]);

	for (unsigned int i = 0; i < numitems; i++)
		indices.push_back(auiIndices[i]);

	gridMesh->initialize(verts, indices);
	gridMesh->create_buffers();

	delete[] aoVertices;
	delete[] auiIndices;
}

void LightingApp::drawMesh(unsigned drawfill, unsigned drawstyle, Mesh* sphereMesh)
{
	shader->bind();
	sphereMesh->bind();
	unsigned int pvt = glGetUniformLocation(shader->program(), "projectionViewWorldMatrix");
	glUniformMatrix4fv(pvt, 1, false, glm::value_ptr(cam->getProjectionView()));

	glPolygonMode(GL_FRONT_AND_BACK, drawfill);
	glDrawElements(drawstyle, sphereMesh->index_count, GL_UNSIGNED_INT, nullptr);

	sphereMesh->unbind();
	shader->unbind();
}

void LightingApp::generateSphere(unsigned int segments, unsigned int rings,
	unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
	unsigned int& indexCount) {

	unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

	// using AIEVertex for now, but could be any struct as long as it has the correct elements
	Vertex* vertices = new Vertex[vertCount];
	unsigned int* indices = new unsigned int[indexCount];

	float ringAngle = glm::pi<float>() / (rings + 1);
	float segmentAngle = 2.0f * glm::pi<float>() / segments;

	Vertex* vertex = vertices;

	for (unsigned int ring = 0; ring < (rings + 2); ++ring) {
		float r0 = glm::sin(ring * ringAngle);
		float y0 = glm::cos(ring * ringAngle);

		for (unsigned int segment = 0; segment < (segments + 1); ++segment, ++vertex) {
			float x0 = r0 * glm::sin(segment * segmentAngle);
			float z0 = r0 * glm::cos(segment * segmentAngle);

			vertex->position = glm::vec4(x0 * 1.5f, y0 *1.5f, z0 * 1.5f, 1);
			vertex->normal = glm::vec4(x0, y0, z0, 0);

			vertex->tangent = glm::vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);

			// not a part of the AIEVertex, but this is how w generate bitangents
			vertex->bitangent = glm::vec4(glm::cross(glm::vec3(vertex->normal), glm::vec3(vertex->tangent)), 0);

			vertex->texcoord = glm::vec2(segment / (float)segments, ring / (float)(rings + 1));
		}
	}

	unsigned int index = 0;
	for (unsigned i = 0; i < (rings + 1); ++i) {
		for (unsigned j = 0; j < segments; ++j) {
			indices[index++] = i * (segments + 1) + j;
			indices[index++] = (i + 1) * (segments + 1) + j;
			indices[index++] = i * (segments + 1) + (j + 1);

			indices[index++] = (i + 1) * (segments + 1) + (j + 1);
			indices[index++] = i * (segments + 1) + (j + 1);
			indices[index++] = (i + 1) * (segments + 1) + j;
		}
	}

	// generate buffers
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1, &vao);

	// all changes will apply to this handle
	glBindVertexArray(vao);

	// set vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));
	// normals
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	// texcoords
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	// tangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3 + sizeof(glm::vec2)));

	// safety
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indices;
	delete[] vertices;
}
