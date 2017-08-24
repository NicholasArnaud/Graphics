#include "IntrodutionApp.h"
#include "Gizmos.h"
#include <GLFW/glfw3.h>
#include "Camera.h"

IntrodutionApp::IntrodutionApp()
{
}


IntrodutionApp::~IntrodutionApp()
{
}

void IntrodutionApp::startup()
{
}

void IntrodutionApp::shutdown()
{
}

void IntrodutionApp::update(float)
{
}

void IntrodutionApp::draw()
{
	Camera* cam = new Camera();
	mat4 s1 = mat4(1);
	vec4 center = vec4(0, 0, 0, 1);
	vec4 color = vec4(0, 0, 0, 0);
	Gizmos::clear();
	Gizmos::addSphere(s1[3], 1, 20, 20, color);
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}
	cam->setLookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));
	Gizmos::draw(cam->getProjectionView());
}


void IntrodutionApp::run(const char * title, unsigned int width, unsigned int height, bool fullscreen)
{
	Application::run(title, width, height, fullscreen);
}
