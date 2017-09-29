#include "TextureApplication.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <gl_core_4_4.h>
#include <Mesh.h>
#include <Shader.h>
#include <Camera.h>
#include <GLFW/glfw3.h>


TextureApplication::TextureApplication(): texture1(0), texture2(0), texture3(0), sphereMesh(nullptr)
{
	sphereMesh = new Mesh();
	planeMesh = new Mesh();
	shader = new Shader();
	crateShader = new Shader();
	cam = new Camera();
	cam->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 1.0f, 1000.f);
}


TextureApplication::~TextureApplication()
{
}

void TextureApplication::startup()
{
	cam->setLookAt(glm::vec3(0.5f,0.5f,5), glm::vec3(.1f), glm::vec3(0, 1, 0));

	shader->load("TexPlusLightVertex.vert",GL_VERTEX_SHADER);
	shader->load("TexPlusLightFragment.frag", GL_FRAGMENT_SHADER);
	crateShader->load("TextureVertex.vert", GL_VERTEX_SHADER);
	crateShader->load("TextureFragment.frag", GL_FRAGMENT_SHADER);
	
	generateSphere(100, 100, sphereMesh->m_VAO,sphereMesh->m_VBO,sphereMesh->m_IBO,sphereMesh->index_count);
	/*
	* glGenTextures
	* glBindTextures
	* glTexImage2D
	* glTexParameteri
	* glTexParameteri
	*/

	genTexPlane();
	int width, height, nrchannals;
	unsigned char* data = stbi_load("images/erf.png", &width, &height, &nrchannals, 0);
	
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);

	unsigned char* data1 = stbi_load("images/erfcloud.png", &width, &height, &nrchannals, 0);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data1);

	unsigned char* data2 = stbi_load("images/crate.png", &width, &height, &nrchannals, 0);
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data2);


}

void TextureApplication::shutdown()
{

}

void TextureApplication::update(float)
{
#pragma region KeyEvents
	glfwPollEvents();
	if (glfwGetKey(m_window, GLFW_KEY_W))
		cam->setPosition(cam->getWorldTransform()[3] -= cam->getWorldTransform()[2] * .2f);
	if (glfwGetKey(m_window, GLFW_KEY_S))
		cam->setPosition(cam->getWorldTransform()[3] += cam->getWorldTransform()[2] * .2f);
	if (glfwGetKey(m_window, GLFW_KEY_A))
		cam->setPosition(cam->getWorldTransform()[3] -= cam->getWorldTransform()[0] * .2f);
	if (glfwGetKey(m_window, GLFW_KEY_D))
		cam->setPosition(cam->getWorldTransform()[3] += cam->getWorldTransform()[0] * .2f);

	static double siPrevMouseX = 0, siPrevMouseY = 0;
	double mouseX = 0, mouseY = 0;
	double iDeltaX = 0, iDeltaY = 0;
	static bool sbMouseButtonDown = false;
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		
		if (sbMouseButtonDown == false)
		{
			sbMouseButtonDown = true;
			glfwGetCursorPos(m_window, &siPrevMouseX, &siPrevMouseY);
		}
		glfwGetCursorPos(m_window, &mouseX, &mouseY);

		iDeltaX = mouseX - siPrevMouseX;
		iDeltaY = mouseY - siPrevMouseY;

		siPrevMouseX = mouseX;
		siPrevMouseY = mouseY;

		glm::mat4 rEle = rotate((float)iDeltaX * 1 / 800, glm::vec3(0, 1, 0));
		glm::mat4 rAzi = rotate((float)iDeltaY * 1 / 800, glm::vec3(1, 0, 0));
		cam->m_viewTransform = rEle* rAzi * cam->m_viewTransform;
		cam->m_worldTransform = glm::inverse(cam->m_viewTransform);
	}
#pragma endregion 
}

void TextureApplication::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	shader->bind();
	glm::mat4 pvm = cam->getProjectionView();
	int matUniform = shader->getUniform("projectionView");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &pvm[0][0]);

	GLint texLoc = shader->getUniform("texture1");
	glUniform1i(texLoc, 0);
	texLoc = shader->getUniform("texture2");
	glUniform1i(texLoc, 1);



	// glActiveTexture
	// glBindTexture
	// glBindVertex Array
	// glDrawElements

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	sphereMesh->bind();
	glDrawElements(GL_TRIANGLES, sphereMesh->index_count, GL_UNSIGNED_INT, 0);
	sphereMesh->unbind();
	shader->unbind();

	crateShader->bind();
	matUniform = shader->getUniform("projectionView");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &pvm[0][0]);

	texLoc = crateShader->getUniform("texture1");
	glUniform1i(texLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture3);

	planeMesh->bind();
	glDrawElements(GL_TRIANGLES, planeMesh->index_count, GL_UNSIGNED_INT, 0);
	planeMesh->unbind();
	crateShader->unbind();
}

void TextureApplication::genTexPlane()
{

	float verts[] = {
		//pos		color		UV
		0, 0, 0,   0, 0, 1,   0, 1,
		0, 1, 0,   1, 1, 0,   0, 0,
		1, 1, 0,   1, 0, 0,   1, 0,
		1, 0, 0,   0, 1, 0,   1, 1
	};

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3 
	};

	planeMesh->index_count =sizeof(indices);
	// generate buffers
	glGenBuffers(1, &planeMesh->m_VBO);
	glGenBuffers(1, &planeMesh->m_IBO);

	// generate vertex array object (descriptors)
	glGenVertexArrays(1,&planeMesh->m_VAO);

	// all changes will apply to this handle
	glBindVertexArray(planeMesh->m_VAO);

	// set vertex buffer data

	glBindBuffer(GL_ARRAY_BUFFER, planeMesh->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeMesh->m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void TextureApplication::generateSphere(unsigned int segments, unsigned int rings,
	unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
	unsigned int& indexCount) {

	unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

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