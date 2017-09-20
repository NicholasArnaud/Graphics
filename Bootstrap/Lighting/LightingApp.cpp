#include "LightingApp.h"
#include "Camera.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>



LightingApp::LightingApp() : mesh(nullptr), shader(nullptr), cam(nullptr)
{
	mesh = new Mesh();
	shader = new Shader();
	cam = new Camera();
	geoapp = new GeometryApplication();
	cam->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 1.0f, 1000.f);
}


LightingApp::~LightingApp()
{
}

void LightingApp::startup()
{
	
	cam->setLookAt(glm::vec3(0, 0, 1), glm::vec3(1), glm::vec3(0, 1, 0));
	shader->load("Vertex.vert", GL_VERTEX_SHADER);
	shader->load("Fragment.frag", GL_FRAGMENT_SHADER);

	GenGrid();
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
	geoapp->drawMesh(GL_LINE, GL_TRIANGLES, mesh);
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

	mesh->initialize(verts, indices);
	mesh->create_buffers();

	delete[] aoVertices;
	delete[] auiIndices;
}