#include "GeometryApplication.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Shader.h"
#include <wingdi.h>
#define PI 3.141592653

GeometryApplication::GeometryApplication() : generalMesh(nullptr), shader(nullptr)
{
	camera = new Camera();
	generalMesh = new Mesh();
	sphereMesh = new Mesh();
	cubeMesh = new Mesh();
	triangleMesh = new Mesh();
	planeMesh = new Mesh();
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
	shader->load("Fragment.frag", GL_FRAGMENT_SHADER);


	//NOTE: Each mesh can only be used for one object
	GenObject(triangleMesh, 1, 0, 0);
	GenObject(cubeMesh, 3, 0, 0);
	GenObject(sphereMesh, 4, 25, 25);
	GenObject(planeMesh, 5, 10, 10);

	GenObject(generalMesh, 6, 0, 0);
}

void GeometryApplication::shutdown()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

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

	drawMesh(GL_LINE, GL_TRIANGLES, generalMesh);

	drawMesh(GL_FILL, GL_TRIANGLE_STRIP, sphereMesh);

	drawMesh(GL_LINE, GL_TRIANGLES, planeMesh);

	drawMesh(GL_LINE, GL_TRIANGLES, triangleMesh);

	drawMesh(GL_FILL, GL_TRIANGLES, cubeMesh);

	
}

/**
 * \brief Binds mesh and shader and Draws the objects
 * \param drawfill USE: GL_LINE, GL_FILL
 * \param drawstyle USE: GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_LINES
 * \param mesh use a mesh that corresponds with the object being generated 
 */
void GeometryApplication::drawMesh(unsigned drawfill,unsigned drawstyle, Mesh* mesh)
{
	shader->bind();
	mesh->bind();
	unsigned int pvt = glGetUniformLocation(shader->program(), "projectionViewWorldMatrix");
	glUniformMatrix4fv(pvt, 1, false, glm::value_ptr(camera->getProjectionView()));

	glPolygonMode(GL_FRONT_AND_BACK, drawfill);
	glDrawElements(drawstyle, mesh->index_count, GL_UNSIGNED_INT, nullptr);

	mesh->unbind();
	shader->unbind();
}

/**
 * \brief Gathers the points and indicies for objects and binds them to a mesh
 * \param mesh select mesh according to the object you want to generate
 * \param select 0= teacher example, 1= 2D Right Triangle, 2 = Flat Square, 3 = Cube, 4= Sphere, 5= Tutorial Generated Plane
 * \param numP = number of points; 3 or more points needed only for sphere
 * \param numM = number of meridians; 3 or more meridians needed only for sphere
 */
void GeometryApplication::GenObject(Mesh* mesh, int select, int numP = 3, int numM = 3)
{
	switch (select)
	{
		//wierd example shape
	case 0:
	{
		camera->setLookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		Vertex a = { glm::vec4(-1, 0, 0, 1) , glm::vec4(.1, .1, .1, 1) };//bl	
		Vertex b = { glm::vec4(1, 0, 0, 1) , glm::vec4(.1, .1, .1, 1) };//br
		Vertex c = { glm::vec4(1, -1, 0, 1) , glm::vec4(.1, .1, .1, 1) };//tl
		Vertex d = { glm::vec4(-1, -1, 0, 1) , glm::vec4(1, 0, 0, 1) };//tr
		Vertex e = { glm::vec4(-1, 1, 0, 1) , glm::vec4(0, 0, 1, 1) };//tr	

		std::vector<Vertex> vertices{ a,b,c,d,e };
		std::vector<unsigned int> indices{ 0, 1, 2, 0, 2, 3, 0, 4, 1 };

		mesh->initialize(vertices, indices);
		mesh->create_buffers();
		break;
	}

	//2D Right Triangle
	case 1:
	{
		//changed cam perspective to center object on the screen
		camera->setLookAt(glm::vec3(0, 0.5f, 3), glm::vec3(0, 0.5f, 0), glm::vec3(0, 1, 0));

		Vertex a = { glm::vec4(-2, 0, 0, 1), glm::vec4(1, 0, 0, 1) }; //bl
		Vertex b = { glm::vec4(2, 0, 0, 1), glm::vec4(0, 1, 0, 1) }; //br
		Vertex c = { glm::vec4(0, 2, 0, 1), glm::vec4(0, 0, 1, 1) }; //tr
		std::vector<Vertex> vertices{ a, b, c };
		std::vector<unsigned int> indices{ 0, 1, 2 };
		mesh->initialize(vertices, indices);
		mesh->create_buffers();
		break;
	}

	//2D Square w Mid
	case 2:
	{
		Vertex a = { glm::vec4(0, 0, 0, 1), glm::vec4(1, 0, 0, 1) }; //bl
		Vertex b = { glm::vec4(3, 0, 0, 1), glm::vec4(0, 1, 0, 1) }; //br
		Vertex c = { glm::vec4(3, 0, 3, 1), glm::vec4(0, 0, 1, 1) }; //tr
		Vertex d = { glm::vec4(0, 0, 3, 1), glm::vec4(1, 1, 0, 1) }; //tl
		Vertex e = { glm::vec4(1.5f, 1.5f, 0, 1), glm::vec4(0, 1, 1, 1) }; //mid
		std::vector<Vertex> vertices{ a, b, c, d, e };
		std::vector<unsigned int> indices{
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4 };
		mesh->initialize(vertices, indices);
		mesh->create_buffers();
		break;
	}

	//Draws 3D Square
	case 3:
	{
		//change cam perspective to see all points
		camera->setLookAt(glm::vec3(2, 2, 5), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0));


		Vertex a = { glm::vec4(0,0,0, 1), glm::vec4(1, 0, 0, 1) }; //front-bleft
		Vertex b = { glm::vec4(4, 0, 0, 1), glm::vec4(0, 1, 0, 1) }; //front-bright
		Vertex c = { glm::vec4(4, 4, 0, 1), glm::vec4(0, 0, 1, 1) }; //front-trright
		Vertex d = { glm::vec4(0, 4, 0, 1), glm::vec4(1, 1, 0, 1) }; //front-topleft

		Vertex e = { glm::vec4(0, 0, -4, 1), glm::vec4(1, 1, 0, 1) }; //bottom-backleft
		Vertex f = { glm::vec4(4, 0, -4, 1), glm::vec4(0, 1, 1, 1) }; //bottom-backright

		Vertex g = { glm::vec4(4, 4, -4, 1), glm::vec4(0, 1, 1, 1) }; //top-backleft
		Vertex h = { glm::vec4(0, 4, -4, 1), glm::vec4(1, 1, 1, 1) }; //top-backright

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
		break;
	}

	//Draws circle
	case 4:
	{
		//Centered camera on center
		camera->setLookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

		float rad = 1;
		std::vector<Vertex> points;
		std::vector<unsigned int> indices;

		if (numP < 3) numP = 3;
		if (numM < 3) numM = 3;

		//For Half Circle
		for (int i = 0; i < numP; i++)
		{
			float slice = PI / (numP - 1);
			float Theta = i * slice;
			float x = rad * sin(Theta);
			float y = rad * cos(Theta);
			Vertex g = { glm::vec4(x, y, 0, 1), glm::vec4(1, 0, 0, 1) };
			printf("Points %i: %f, %f, %f \n", points.size(), g.position.x, g.position.y, g.position.z);
			points.push_back(g);
		}

		//For Complete Circle
		float phi = 2.f* PI / numM;
		for (int l = 0; l <= numM; l++)
		{
			for (int k = 0; k < numP; k++)
			{
				float newX = rad* points[k].position.x * cos(phi* l);
				float newY = rad* points[k].position.y;
				float newZ = rad* points[k].position.x * -sin(phi* l);
				Vertex n = { glm::vec4(newX, newY, newZ, 1), glm::vec4(1, 0, 0, 1) };
				printf("Points %i: %f, %f, %f \n", points.size(), n.position.x, n.position.y, n.position.z);
				points.push_back(n);
			}
		}


		for (int i = 0; i <= numM; i++)
		{
			for (int j = 0; j < numP; j++)
			{
				indices.push_back(i* numM + j); //btm left
				indices.push_back(i* numM + j + numM); //btm right
			}
		}

		mesh->initialize(points, indices);
		mesh->create_buffers();
		break;
	}

	//Draws grid used in tutorial
	case 5:
	{
		Vertex * aoVertices = new Vertex[100];
		for (unsigned int r = 0; r < numP; ++r)
			for (unsigned int c = 0; c < numM; ++c)
				aoVertices[r * numM + c].position = glm::vec4((float)c, 0, (float)r, 1);
		const unsigned int numitems = (numP - 1) * (numM - 1) * 6;
		auto auiIndices = new unsigned int[numitems];

		unsigned int index = 0;
		for (unsigned int r = 0; r < numP - 1; ++r)
			for (unsigned int c = 0; c < numM - 1; ++c)
			{
				//Triangle 1
				auiIndices[index++] = r * numM + c;
				auiIndices[index++] = (r + 1) * numM + c;
				auiIndices[index++] = (r + 1) * numM + (c + 1);
				//Triangle 2
				auiIndices[index++] = r * numM + c;
				auiIndices[index++] = (r + 1) * numM + (c + 1);
				auiIndices[index++] = r * numM + (c + 1);
			}

		std::vector<Vertex> verts;
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < numP * numM; i++)
			verts.push_back(aoVertices[i]);

		for (unsigned int i = 0; i < numitems; i++)
			indices.push_back(auiIndices[i]);

		mesh->initialize(verts, indices);
		mesh->create_buffers();

		delete[] aoVertices;
		delete[] auiIndices;

		break;
	}

	case 6:
		{
			generateSphere(15, 15, mesh->m_VAO, mesh->m_VBO, mesh->m_IBO, mesh->index_count);
			break;
		}
	default: break;
	}
}

void GeometryApplication::generateSphere(unsigned int segments, unsigned int rings,
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