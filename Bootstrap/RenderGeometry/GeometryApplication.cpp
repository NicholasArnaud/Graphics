#include "GeometryApplication.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"
#define PI 3.141592653

GeometryApplication::GeometryApplication() : m_programID(0), mesh(nullptr)
{
	camera = new Camera();
	mesh = new Mesh();
	shader = new Shader();
	camera->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 0.1f, 1000.f);
}

GeometryApplication::~GeometryApplication()
{
}

void GeometryApplication::startup()
{
	camera->setLookAt(glm::vec3(0, 0, 1), glm::vec3(1), glm::vec3(0, 1, 0));


	const char* vsSource = "#version 410\n \
	layout(location=0) in vec4 position; \
	layout(location=1) in vec4 colour; \
	out vec4 vColour; \
	uniform mat4 projectionViewWorldMatrix; \
	void main() { vColour = colour; gl_Position =\
	projectionViewWorldMatrix * position; }";
	const char* fsSource = "#version 410\n \
	in vec4 vColour; \
	out vec4 fragColour;\
	void main() { fragColour = vColour; }";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	int success = GL_FALSE;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_programID, infoLogLength, nullptr, infoLog);
		printf("ERROR: Failed to link shader program! \n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	//shader->defaultLoad();
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}

void GeometryApplication::shutdown()
{
}

void GeometryApplication::update(float)
{

}

void GeometryApplication::draw()
{
	mesh->bind();

	unsigned int projectionViewTransform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	glUniformMatrix4fv(projectionViewTransform, 1, false, value_ptr(camera->getProjectionView()));
	glDrawElements(GL_TRIANGLE_STRIP, mesh->index_count, GL_UNSIGNED_INT, nullptr);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	mesh->unbind();
}

void GeometryApplication::GenObject(int numb)
{
	if (numb == 0)
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

	if (numb == 1)
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

	if (numb == 2)
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

	if (numb == 3)
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

	if (numb == 4)
	{
		camera->setLookAt(glm::vec3(0, 1, 10), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
		//Triangle strips
		std::vector<unsigned int> indices{ 0 };
		
		int xrows = 2;
		int yrows = 2;
		Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(1, 0, 0, 1) }; //bl
		Vertex b = { glm::vec4(10, 0, 0, 1), glm::vec4(0, 1, 0, 1) }; //br
		Vertex c = { glm::vec4(10, 10, 0, 1), glm::vec4(0, 0, 1, 1) }; //tr
		Vertex d = { glm::vec4(0, 10, 0, 1), glm::vec4(1, 1, 0, 1) }; //tl
		
		std::vector<Vertex> points{a, b, d, c};
		for (int i = 0; i <= xrows; i++)
		{
			for (int j = 0; j < yrows; j++)
			{
				indices.push_back(i* xrows + j); //btm left
				indices.push_back(i* xrows + j + xrows); //btm right
			}
		}

		mesh->initialize(points, indices);
		mesh->create_buffers();
	}

	if (numb == 5)
	{
		//Centered camera on center
		camera->setLookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


		int np = 25;
		int nm = 25;
		int rad = 1;
		std::vector<Vertex> points;
		std::vector<unsigned int> indices;


		//For Half Circle
		for (int i = 0; i < np; i++)
		{
			float slice = PI / (np - 1);
			float Theta = i * slice;
			float x = rad * sin(Theta);
			float y = rad * cos(Theta);
			Vertex g = { glm::vec4(x, y, 0, 1), glm::vec4(0.5f, 0.5f, 0.5f, 1) };
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
				Vertex n = { glm::vec4(newX, newY, newZ, 1), glm::vec4(0.5f, 0.5f, 0.5f, 1) };
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

void GeometryApplication::run(const char* title, unsigned width, unsigned height, bool fullscreen)
{
	glfwInit();

	float currTime = glfwGetTime();
	float prevTime = 0;
	float deltaTime = currTime - prevTime;
	glfwPollEvents();

	GLFWwindow* m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return;
	}

	startup();
	GenObject(4);


	glUseProgram(m_programID);

	glClearColor(0, 0, 0, 1.0f);
	glEnable(GL_DEPTH_TEST);
	while (!m_gameover)
	{
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;
		prevTime = currTime;
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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
			camera->m_viewTransform = glm::inverse(camera->m_worldTransform);
		}
		
		draw();
		glfwSwapBuffers(m_window);
		m_gameover = (glfwWindowShouldClose(m_window) == GLFW_TRUE);
	}
}
