#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <imgui.h>
#include <assert.h>
#include <iostream>
using namespace std;
int main ()
{
	glm::vec3 v = glm::vec3(1, 1, 1);
	v.r = 5.f;
	v.x = 20.f;
	glm::vec2 textureCoordinates = glm::vec2(1, 0);

	if(!glfwInit())
	{
		printf("no glfw!");
	}
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
	if(window == nullptr)
	{
		glfwTerminate();
		return -2;
	}
	assert(window != nullptr);
	glfwMakeContextCurrent(window);

	if(ogl_LoadFunctions()== ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	
	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	
	printf("GL: %i.%i\n", major, minor);
	glClearColor(0.5f, 0.5f, 1.f, 1.f);
	glm::vec4 clearcolor = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);

	while (true)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		if (glfwWindowShouldClose(window))
			break;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, true);
		if(glfwGetKey(window, GLFW_KEY_F))
		{
			clearcolor.r -= 0.001f;
			clearcolor.g -= 0.001f;
			clearcolor.b -= 0.001f;
			printf("clearcolor r => %f", clearcolor.r);
		}
		glClearColor(clearcolor.r, clearcolor.g, clearcolor.b, clearcolor.a);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}