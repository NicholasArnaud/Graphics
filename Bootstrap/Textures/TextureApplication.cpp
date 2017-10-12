#include "TextureApplication.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <gl_core_4_4.h>
#include <Mesh.h>
#include <Shader.h>
#include <Camera.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include<iostream>

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

float interpolate(float a, float b, float x)
{
	float ft = x * 3.1415927;
	float f = (1.0 - cos(ft))*0.5;
	return a*(1.0 - f) + sin(b*f)*0.5;
}

double noise(int x, int y)
{
	int rnd = rand() % 30;
	int a = prime[rnd][rand() % 3];
	int b = prime[rnd][rand() % 3];
	int c= prime[rnd][rand() % 3];
	int n = x +y*57;
	n = n << 13 ^ n;
	int nn = 1.0-(n*(n*n * a + b)+c & 0x7ffffff) / 1073741824.0;
	//must be within -1 to 1
	return nn;
}

double nick_perlin(glm::vec2 pos)
{
	double floorx = floor(pos.x);
	double floory = floor(pos.y);

	double s = noise(floorx, floory);
	double t = noise(floorx + 1, floory);
	double u = noise(floorx, floory + 1);
	double v = noise(floorx + 1, floory + 1);

	double int1 = interpolate(s, t, pos.x - floorx);
	double int2 = interpolate(u, v, pos.x - floorx);
	return interpolate(int1,int2, pos.y - floory);
}





TextureApplication::TextureApplication() : texture1(0), texture2(0), texture3(0), perlin_data(0), texture4(0)
{
	sphereMesh = new Mesh();
	planeMesh = new Mesh();
	shader = new Shader();
	planeShader = new Shader();
	cam = new Camera();
	cam->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 1.0f, 1000.f);
}

TextureApplication::~TextureApplication()
{
}

Mesh* TextureApplication::generateGrid(unsigned int rows, unsigned int cols)
{
	auto aoVertices = new Vertex[rows * cols];
	for (unsigned int r = 0; r < rows; ++r)
	{
		for (unsigned int c = 0; c < cols; ++c)
		{
			Vertex verts = {
				glm::vec4(float(c), 0, float(r), 1),
				glm::vec4(sin(r), cos(c), 0, 1),
				glm::vec4(0, 1, 0, 0),
				glm::vec2(float(c) / float(cols -1), float(r) / float(rows -1))
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

void TextureApplication::startup()
{
	cam->setLookAt(glm::vec3(35,35,75), glm::vec3(35,0,35), glm::vec3(0, 1, 0));

	//shader->load("TexPlusLightVertex.vert", GL_VERTEX_SHADER);
	//shader->load("TexPlusLightFragment.frag", GL_FRAGMENT_SHADER);
	planeShader->load("NoiseVert.vert", GL_VERTEX_SHADER);
	planeShader->load("NoiseFragment.frag", GL_FRAGMENT_SHADER);
	
	//crateShader->load("TextureVertex.vert", GL_VERTEX_SHADER);
	//crateShader->load("TextureFragment.frag", GL_FRAGMENT_SHADER);
	shader->load("NoiseVert.vert", GL_VERTEX_SHADER);
	shader->load("NoiseFragment.frag", GL_FRAGMENT_SHADER);

	/*
	* glGenTextures
	* glBindTextures
	* glTexImage2D
	* glTexParameteri
	* glTexParameteri
	*/
	//generateSphere(100, 100, sphereMesh->m_VAO, sphereMesh->m_VBO, sphereMesh->m_IBO, sphereMesh->index_count);
	
	
	//setupTexture("images/earth_diffuse.jpg", &texture1);
	//setupTexture("images/earth_cloud.jpg", &texture2);
	//setupTexture("images/crate.png", &texture3);

	
	genNoiseTex(64, 64);
	planeMesh = generateGrid(64, 64);
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

	glm::mat4 pvm = cam->getProjectionView();
#pragma region plane
	planeShader->bind();
	int matUniform = shader->getUniform("projectionView");
	unsigned int perlin = shader->getUniform("perlinTexture");
	
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &pvm[0][0]);
	//texture4 = perlin_data;
	//drawTex(crateShader, &texture4, 1);
	planeMesh->bind();
	glUniform1i(perlin, 0);
	glDrawElements(GL_TRIANGLES, planeMesh->index_count, GL_UNSIGNED_INT, 0);
	planeMesh->unbind();
	planeShader->unbind();
#pragma endregion 

/*	
#pragma region sphere

	shader->bind();

	matUniform = shader->getUniform("projectionView");
	glUniformMatrix4fv(matUniform, 1, GL_FALSE, &pvm[0][0]);

	texture4 = perlin_data;
	drawTex(shader, &texture4, 1);

	sphereMesh->bind();
	glDrawElements(GL_TRIANGLES, sphereMesh->index_count, GL_UNSIGNED_INT, 0);
	sphereMesh->unbind();
	shader->unbind();
#pragma endregion
	*/
}


void TextureApplication::generateSphere(const unsigned int segments, const unsigned int rings,
	unsigned int& vao, unsigned int& vbo, unsigned int& ibo,
	unsigned int& indexCount) const
{
	const unsigned int vertCount = (segments + 1) * (rings + 2);
	indexCount = segments * (rings + 1) * 6;

	Vertex* vertices = new Vertex[vertCount];
	unsigned int* indices = new unsigned int[indexCount];

	const float ringAngle = glm::pi<float>() / (rings + 1);
	const float segmentAngle = 2.0f * glm::pi<float>() / segments;

	Vertex* vertex = vertices;

	for (unsigned int ring = 0; ring < (rings + 2); ++ring)
	{
		float r0 = glm::sin(ring * ringAngle);
		float y0 = glm::cos(ring * ringAngle);

		for (unsigned int segment = 0; segment < (segments + 1); ++segment, ++vertex)
		{
			float x0 = r0 * glm::sin(segment * segmentAngle);
			float z0 = r0 * glm::cos(segment * segmentAngle);

			vertex->position = glm::vec4(x0 * 1.5f, y0 *1.5f, z0 * 1.5f, 1);
			vertex->normal = glm::vec4(x0, y0, z0, 0);
			vertex->tangent = glm::vec4(glm::sin(segment * segmentAngle + glm::half_pi<float>()), 0, glm::cos(segment * segmentAngle + glm::half_pi<float>()), 0);
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


void TextureApplication::setupTexture(char* imageLoc, unsigned int* texture)
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

void TextureApplication::drawTex(Shader* shader, unsigned int* texture, const int numOfTextures)
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

float* TextureApplication::genNoiseTex(unsigned int width, unsigned int height)
{
	int dims = 64; 
	float *perlinData = new float[dims * dims];
	float scale = (1.0f / dims) * 3;
	int octaves = 16;
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			float amplitude = 1.f;
			float persistence = 0.2f;

			perlinData[y * dims + x] = 0;
			for (int o = 0; o < octaves; ++o)
			{
				float freq = powf(2, (float)o);
				float perlinSample = glm::perlin(glm::vec2((float)x, (float)y) * scale * freq) * 0.5f + 0.5f;
				perlinSample = nick_perlin(glm::vec2((float)x, (float)y) * scale * freq)* 0.5f + 0.5f;
				perlinData[y * dims + x] += perlinSample * amplitude;
				amplitude *= persistence;
			}
		}
	}

	glGenTextures(1, &perlin_data);
	glBindTexture(GL_TEXTURE_2D, perlin_data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width,height, 0, GL_RED, GL_FLOAT, perlinData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	float* one = new float(10.1f);
	return one;
}


