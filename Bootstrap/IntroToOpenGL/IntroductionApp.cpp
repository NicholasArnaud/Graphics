#include "IntroductionApp.h"
#include "Gizmos.h"
#include <glm.hpp>
#include <ext.hpp>

IntroductionApp::IntroductionApp()
{
}


IntroductionApp::~IntroductionApp()
{
}

void IntroductionApp::startup()
{
	m_view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);
	Gizmos::create();
	
}

void IntroductionApp::shutdown()
{
}

void IntroductionApp::update(float)
{
}

void IntroductionApp::draw()
{
	
	Gizmos::addTransform(glm::mat4(1));
	Gizmos::addSphere(glm::vec3(0, 0, 0), 5.f, 20, 20, glm::vec4(1, .25f, 1, 1));
	Gizmos::draw(m_projection * m_view);	
	Gizmos::clear();
}

void IntroductionApp::run(const char * title, unsigned int width, unsigned int height, bool fullscreen)
{
	Application::run(title, width, height, fullscreen);
}
