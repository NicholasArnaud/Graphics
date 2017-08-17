#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <imgui.h>
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
	while (true)
	{
		glfwPollEvents();
		if (glfwWindowShouldClose(window))
			break;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(window, true);
	}
	return 0;
}