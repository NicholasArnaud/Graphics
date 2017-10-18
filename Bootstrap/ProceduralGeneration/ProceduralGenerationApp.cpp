#include "ProceduralGenerationApp.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <math.h>
#include <sstream>
#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"



void setupTexture(char* imageLoc, unsigned int* texture)
{
	int width, height, nrchannals;
	unsigned char* data = stbi_load(imageLoc, &width, &height, &nrchannals, 0);

	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	stbi_image_free(data);
}

void drawTex(Shader* shader, unsigned int* texture, const int numOfTextures)
{
	std::stringstream ss;
	for (int i = 0; i <= numOfTextures; i++)
	{
		ss << "texture" << i + 1;
		std::string tmp = ss.str();
		const char * tex = tmp.c_str();

		GLint texLoc = shader->getUniform(tex);
		glUniform1i(texLoc, i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, *texture + i);

		ss.str(std::string());
		ss.clear();
	}
}


ProceduralGenerationApp::ProceduralGenerationApp(): shader(nullptr), mesh(nullptr), cam(nullptr), perlin_data(0)
{
	cam = new Camera();
	shader = new Shader();
	mesh = new Mesh();

	cam->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 1.0f, 1000.f);
}


ProceduralGenerationApp::~ProceduralGenerationApp()
{
}

void ProceduralGenerationApp::startup()
{
	cam->setLookAt(glm::vec3(35, 35, 75), glm::vec3(35, 0, 35), glm::vec3(0, 1, 0));

	shader->load("NoiseVert.vert", GL_VERTEX_SHADER);
	shader->load("NoiseFragment.frag", GL_FRAGMENT_SHADER);
	genNoiseTex(64, 64);
	mesh = generateGrid(64, 64);
}

void ProceduralGenerationApp::shutdown()
{
}

void ProceduralGenerationApp::update(float)
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

void ProceduralGenerationApp::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glm::mat4 pvm = cam->getProjectionView();

#pragma region plane
	shader->bind();
	int matUniform = shader->getUniform("projectionView");
	unsigned int perlin = shader->getUniform("perlinTexture");

	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &pvm[0][0]);
	mesh->bind();
	glUniform1i(perlin, 0);
	glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
	mesh->unbind();
	shader->unbind();
#pragma endregion 
}



int prime[][3] = {
	995615039, 600173719, 701464987 ,
	831731269, 162318869, 136250887 ,
	174329291, 946737083, 245679977 ,
	362489573, 795918041, 350777237 ,
	457025711, 880830799, 909678923 ,
	787070341, 177340217, 593320781 ,
	405493717, 291031019, 391950901 ,
	458904767, 676625681, 424452397 ,
	531736441, 939683957, 810651871 ,
	997169939, 842027887, 423882827
};

float ProceduralGenerationApp::interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1.0 - cos(ft))*x;
	return a*(1.0 - f) + sin(b* f)*-x;
}

double ProceduralGenerationApp::noise(int x, int y)
{
	int rnd = rand() % 30;
	int a = prime[rnd][rand() % 3];
	int b = prime[rnd][rand() % 3];
	int c = prime[rnd][rand() % 3];
	int n = (x + y) * 57;
	n = n << 13 ^ n;
	int nn = 1.0 - (n*(n*n * a + b) + c & 0x7ffffff) / 1073741824.0;
	//must be within -1 to 1
	return nn;
}



double ProceduralGenerationApp::nick_perlin(glm::vec2 pos)
{
	double floorx = floor(pos.x);
	double floory = floor(pos.y);

	double s = this->noise(floorx, floory);
	double t = this->noise(floorx + 1, floory);
	double u = this->noise(floorx, floory + 1);
	double v = this->noise(floorx + 1, floory + 1);

	double int1 = this->interpolate(s, t, pos.x - floorx);
	double int2 = this->interpolate(u, v, pos.x - floorx);
	return this->interpolate(int1, int2, pos.y - floory);
}

float* ProceduralGenerationApp::genNoiseTex(unsigned int width, unsigned int height)
{
	int dims = 64;
	float *perlinData = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 6;
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.3f;

			perlinData[y * dims + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				//float perlinSample = glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				float perlinSample = nick_perlin(glm::vec2((float)x, (float)y) * scale * freq)* 0.5f + 0.5f;
				perlinData[y * dims + x] += perlinSample * amplitude;
				amplitude *= persistence;
			}
		}
	}
	glGenTextures(1, &perlin_data);
	glBindTexture(GL_TEXTURE_2D, perlin_data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, perlinData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	float* one = new float(10.1f);
	return one;
}

Mesh* ProceduralGenerationApp::generateGrid(unsigned int rows, unsigned int cols)
{
	//Double for loop for Vertex assignment
	auto aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			Vertex verts = {
				glm::vec4(float(c), 0, float(r), 1),
				glm::vec4(sin(r), cos(c), 0, 1),
				glm::vec4(0, 1, 0, 0),
				glm::vec2(float(c) / float(cols - 1), float(r) / float(rows - 1))
			};
			aoVertices[r * cols + c] = verts;
		}
	}

	std::vector<Vertex> verts = std::vector<Vertex>();
	std::vector<unsigned int> indices = std::vector<unsigned int>();

	//Defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[(rows - 1) * (cols - 1) * 6];
	unsigned int index = 0;
	for (unsigned int r = 0; r < (rows - 1); ++r)
	{
		for (unsigned int c = 0; c < (cols - 1); ++c)
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
	}

	//Create and bind buffers to a vertex array object
	int m_rows = rows;
	int m_cols = cols;
	Mesh* plane = new Mesh();
	for (unsigned int i = 0; i < (rows * cols); i++)
		verts.push_back(aoVertices[i]);
	for (unsigned int i = 0; i < index; i++)
		indices.push_back(auiIndices[i]);
	plane->initialize(verts, indices);
	plane->create_buffers();

	return plane;
}