#include "GeometryApplication.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"
#include <wingdi.h>
#define PI 3.141592653

GeometryApplication::GeometryApplication() : m_programID(0), mesh(nullptr), shader(nullptr)
{
	GLFWwindow * m_window;
	camera = new Camera();
	mesh = new Mesh();
	shader = new Shader();
	camera->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 1.0f, 1000.f);
}

GeometryApplication::~GeometryApplication()
{
}

void GeometryApplication::startup()
{
	
	camera->setLookAt(glm::vec3(0, 0, 1), glm::vec3(1), glm::vec3(0, 1, 0));
	
	shader->load("Vertex.vert", GL_VERTEX_SHADER);
	shader->load("Fragment.frag",GL_FRAGMENT_SHADER);

	GenObject(4, 12, 12);
}

void GeometryApplication::shutdown()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

glm::mat4 sphere = glm::mat4(1);
void GeometryApplication::update(float)
{
	glfwPollEvents();
	if (glfwGetKey(m_window, GLFW_KEY_W))
		camera->setPosition(camera->getWorldTransform()[3] -= camera->getWorldTransform()[2] * .2f);		
	if (glfwGetKey(m_window, GLFW_KEY_S))
		camera->setPosition(camera->getWorldTransform()[3] += camera->getWorldTransform()[2] * .2f);		
	if (glfwGetKey(m_window, GLFW_KEY_A))
		camera->setPosition(camera->getWorldTransform()[3] -= camera->getWorldTransform()[0] * .2f);		
	if (glfwGetKey(m_window, GLFW_KEY_D))
		camera->setPosition(camera->getWorldTransform()[3] += camera->getWorldTransform()[0] * .2f);

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
		camera->m_worldTransform = rEle* rAzi * camera->m_worldTransform;
		camera->m_viewTransform = inverse(camera->m_worldTransform);
	}
}

void GeometryApplication::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//FIRST OBJECT
	shader->bind();
	mesh->bind();

	unsigned int projectionViewTransform = glGetUniformLocation(shader->program(), "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewTransform, 1, false, value_ptr(camera->getProjectionView() * sphere));
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLE_STRIP, mesh->index_count, GL_UNSIGNED_INT, nullptr);

	mesh->unbind();
	shader->unbind();
}

void GeometryApplication::GenObject(int select, int numP =3, int numM =3)
{
	if (select == 0)
	{
		camera->setLookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		//wierd example shape
		Vertex a = { glm::vec4(-1, 0, 0, 1) , glm::vec4(.1, .1, .1, 1) };//bl	
		Vertex b = { glm::vec4(1, 0, 0, 1) , glm::vec4(.1, .1, .1, 1) };//br
		Vertex c = { glm::vec4(1, -1, 0, 1) , glm::vec4(.1, .1, .1, 1) };//tl
		Vertex d = { glm::vec4(-1, -1, 0, 1) , glm::vec4(1, 0, 0, 1) };//tr
		Vertex e = { glm::vec4(-1, 1, 0, 1) , glm::vec4(0, 0, 1, 1) };//tr	

		std::vector<Vertex> vertices{ a,b,c,d,e };
		std::vector<unsigned int> indices{ 0, 1, 2, 0, 2, 3, 0, 4, 1 };

		mesh->initialize(vertices, indices);
		mesh->create_buffers();
	}

	if (select == 1)
	{
		//changed cam perspective to center object on the screen
		camera->setLookAt(glm::vec3(0, 0.5f, 3), glm::vec3(0, 0.5f, 0), glm::vec3(0, 1, 0));

		//2D Right Triangle
		Vertex a = { glm::vec4(-1, 0, 0, 1), glm::vec4(1, 0, 0, 1) }; //bl
		Vertex b = { glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1) }; //br
		Vertex c = { glm::vec4(0, 1, 0, 1), glm::vec4(0, 0, 1, 1) }; //tr
		std::vector<Vertex> vertices{ a, b, c };
		std::vector<unsigned int> indices{ 0, 1, 2 };
		mesh->initialize(vertices, indices);
		mesh->create_buffers();
	}

	if (select == 2)
	{

		//2D Square w Mid
		Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(1, 0, 0, 1) }; //bl
		Vertex b = { glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1) }; //br
		Vertex c = { glm::vec4(1, 1, 0, 1), glm::vec4(0, 0, 1, 1) }; //tr
		Vertex d = { glm::vec4(0, 1, 0, 1), glm::vec4(1, 1, 0, 1) }; //tl
		Vertex e = { glm::vec4(1.5f, 1.5f, 0, 1), glm::vec4(0, 1, 1, 1) }; //mid
		std::vector<Vertex> vertices{ a, b, c, d, e };
		std::vector<unsigned int> indices{
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4 };
		mesh->initialize(vertices, indices);
		mesh->create_buffers();
	}

	if (select == 3)
	{
		//change cam perspective to see all points
		camera->setLookAt(glm::vec3(2, 2, 5), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0));

		//3D Square
		Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(1, 0, 0, 1) }; //front-bleft
		Vertex b = { glm::vec4(1, 0, 0, 1), glm::vec4(0, 1, 0, 1) }; //front-bright
		Vertex c = { glm::vec4(1, 1, 0, 1), glm::vec4(0, 0, 1, 1) }; //front-trright
		Vertex d = { glm::vec4(0, 1, 0, 1), glm::vec4(1, 1, 0, 1) }; //front-topleft

		Vertex e = { glm::vec4(0, 0, -1, 1), glm::vec4(1, 1, 0, 1) }; //bottom-backleft
		Vertex f = { glm::vec4(1, 0, -1, 1), glm::vec4(0, 1, 1, 1) }; //bottom-backright

		Vertex g = { glm::vec4(1, 1, -1, 1), glm::vec4(0, 1, 1, 1) }; //top-backleft
		Vertex h = { glm::vec4(0, 1, -1, 1), glm::vec4(1, 1, 1, 1) }; //top-backright

		std::vector<Vertex> vertices{ a, b, c, d, e, f, g, h };
		std::vector<unsigned int> indices{
			0, 1, 2, //front
			0, 2, 3, //front
			4, 5, 1, //btm
			4, 1, 0, //btm
			5, 4, 7, //back
			5, 7, 6, //back
			3, 2, 6, //top
			3, 6, 7, //top
			4, 0, 3, //left
			4, 3, 7, //left
			1, 5, 6, //right
			1, 6, 2, //right

		};
		mesh->initialize(vertices, indices);
		mesh->create_buffers();
	}

	if (select == 4)
	{
		//Centered camera on center
		camera->setLookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


		int np = numP;
		int nm = numM;
		int rad = 1;
		std::vector<Vertex> points;
		std::vector<unsigned int> indices;

		if (np < 3) np = 3;
		if (nm < 3) nm = 3;

		//For Half Circle
		for (int i = 0; i < np; i++)
		{
			float slice = PI / (np - 1);
			float Theta = i * slice;
			float x = rad * sin(Theta);
			float y = rad * cos(Theta);
			Vertex g = { glm::vec4(x, y, 0, 1), glm::vec4(0.0f, 0.0f, 0.0f, 1) };
			printf("Points %i: %f, %f, %f \n", points.size(), g.position.x, g.position.y, g.position.z);
			points.push_back(g);
		}

		//For Complete Circle

		float phi = 2.f* PI / nm;
		for (int l = 0; l <= nm; l++)
		{
			for (int k = 0; k < np; k++)
			{
				float newX = rad* points[k].position.x * cos(phi* l);
				float newY = rad* points[k].position.y;
				float newZ = rad* points[k].position.x * -sin(phi* l);
				Vertex n = { glm::vec4(newX, newY, newZ, 1), glm::vec4(0.0f, 0.0f, 0.0f, 1) };
				printf("Points %i: %f, %f, %f \n", points.size(), n.position.x, n.position.y, n.position.z);
				points.push_back(n);
			}
		}


		for (int i = 0; i <= nm; i++)
		{
			for (int j = 0; j < np; j++)
			{
				indices.push_back(i* nm + j); //btm left
				indices.push_back(i* nm + j + nm); //btm right
			}
		}

		mesh->initialize(points, indices);
		mesh->create_buffers();
	}
}