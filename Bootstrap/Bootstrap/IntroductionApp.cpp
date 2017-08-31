#include "IntroductionApp.h"
#include "Gizmos.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>

 
 
IntroductionApp::IntroductionApp()
{
}


IntroductionApp::~IntroductionApp()
{
}

void IntroductionApp::run(const char * title, unsigned int width, unsigned int height, bool fullscreen)
{
	Application::run(title, width, height, fullscreen);
}

void IntroductionApp::startup()
{

}

void IntroductionApp::shutdown()
{
}

void IntroductionApp::update(float)
{
}

void IntroductionApp::draw()
{
	glm::mat4 s1 = glm::mat4(1);
	glm::vec4 center = glm::vec4(0, 5, 0, 1);
	glm::vec4 color = glm::vec4(0, 0, 0, 0);
	Gizmos::clear();
	Gizmos::addSphere(s1[3], 1, 20, 20, color);
	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
			glm::vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(glm::vec3(10, 0, -10 + i),
			glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	
}
