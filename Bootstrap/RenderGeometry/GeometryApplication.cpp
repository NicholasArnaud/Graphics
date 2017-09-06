#include "GeometryApplication.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <cstdio>
#include <wingdi.h>


GeometryApplication::GeometryApplication() : m_programID(0), mesh(nullptr)
{
	camera = new Camera();
	mesh = new Mesh();

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
	int success = GL_FALSE;
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
	glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, nullptr);
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
		camera->setLookAt(glm::vec3(15, 15, 45), glm::vec3(15, 15, 0), glm::vec3(0, 1, 0));

		std::vector<unsigned int> indices{0};
		std::vector<Vertex> l;
		int xrows = 25;
		int yrows = 25;

		for (int x = 0; x <= xrows; x++)
		{
			for (int y = 0; y <= yrows; y++)
			{
				Vertex g = { glm::vec4(x, y, 0, 1), glm::vec4(0.5f, 0.5f, 0.5f, 1) };
				
				l.push_back(g);
			}
		}
		for (int x = 0; x <= xrows; x++)
		{
			for (int y = 0; y <= yrows; y++)
			{
				indices.push_back(x *yrows + y);
				indices.push_back((x + 1) * yrows + y);
				indices.push_back((x + 1) * yrows+ (y + 1));

				indices.push_back(x *yrows + y);
				indices.push_back((x + 1) * yrows + y + 1);
				indices.push_back( x* yrows +(y + 1));
				
			}
		}
		
		mesh->initialize(l, indices);
		mesh->create_buffers();
	}
}

void GeometryApplication::run(const char* title, unsigned width, unsigned height, bool fullscreen)
{
	glfwInit();
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
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		draw();

		glfwSwapBuffers(m_window);
		m_gameover = (glfwWindowShouldClose(m_window) == GLFW_TRUE);
	}
}
