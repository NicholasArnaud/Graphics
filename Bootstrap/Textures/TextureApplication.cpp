#include "TextureApplication.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <gl_core_4_4.h>
#include <Mesh.h>
#include <Shader.h>
#include <Camera.h>
#include <GLFW/glfw3.h>


TextureApplication::TextureApplication(): texture(0)
{
	planeMesh = new Mesh();
	shader = new Shader();
	cam = new Camera();
	cam->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 1.0f, 1000.f);
}


TextureApplication::~TextureApplication()
{
}

void TextureApplication::startup()
{
	cam->setLookAt(glm::vec3(0,0,10), glm::vec3(0), glm::vec3(0, 1, 0));

	shader->load("TextureVertex.vert",GL_VERTEX_SHADER);
	shader->load("TextureFragment.frag", GL_FRAGMENT_SHADER);

	genTexPlane(2, 3);
	/*
	* glGenTextures
	* glBindTextures
	* glTexImage2D
	* glTexParameteri
	* glTexParameteri
	*/
	int width, height, nrchannals;
	unsigned char* data = stbi_load("images/crate.png", &width, &height, &nrchannals, 0);
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);


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
	double iDeltaX, iDeltaY;
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

	// glActiveTexture
	// glBindTexture
	// glBindVertex Array
	// glDrawElements

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	planeMesh->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	planeMesh->unbind();
}

void TextureApplication::genTexPlane(int rows, int cols)
{

	float verts[] = {
		//pos		color		UV
		0, 0, 0,   0, 0, 1,   0, 0,
		0, 1, 0,   1, 1, 0,   0, 1,
		1, 1, 0,   1, 0, 0,   1, 1,
		1, 0, 0,   0, 1, 0,   1, 0
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
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	
	// color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	
	// texture coord attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	
/*	
	Vertex * aoVertices = new Vertex[100];
	for (unsigned int r = 0; r < rows; ++r)
		for (unsigned int c = 0; c < cols; ++c)
			aoVertices[r * cols + c].position = glm::vec4((float)c, 0, (float)r, 1);
	const unsigned int numitems = (rows - 1) * (cols - 1) * 6;
	auto auiIndices = new unsigned int[numitems];

	unsigned int index = 0;
	for (unsigned int r = 0; r < rows - 1; ++r)
		for (unsigned int c = 0; c < cols - 1; ++c)
		{
			//Triangle 1
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			//Triangle 2
			auiIndices[index++] = r * cols + c;
			auiIndices[index++] = (r + 1) * cols + (c + 1);
			auiIndices[index++] = r * cols + (c + 1);
		}

	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	for (unsigned int i = 0; i < rows * cols; i++)
		verts.push_back(aoVertices[i]);

	for (unsigned int i = 0; i < numitems; i++)
		indices.push_back(auiIndices[i]);

	planeMesh->initialize(verts, indices);
	planeMesh->create_buffers();

	delete[] aoVertices;
	delete[] auiIndices;
*/
}