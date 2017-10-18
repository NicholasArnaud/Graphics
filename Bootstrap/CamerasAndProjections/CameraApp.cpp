#include "CameraApp.h"
#include "Camera.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <ext.hpp>
#include <Gizmos.h>
#include <assert.h>
#include <iostream>


CameraApp::CameraApp(): realTime(0), deltaTime(0), prevTime(0)
{
	cam = new Camera();
	cam->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 0.1f, 1000.f);
}


CameraApp::~CameraApp()
{
}


void CameraApp::shutdown()
{
}

void CameraApp::startup()
{

	//Creates a starting point for the camera and where it should look
	cam->setLookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	Gizmos::create();
}

void CameraApp::update(float deltatime)
{
	realTime += deltatime;
	printf("PlayTime: %f \n", realTime);

	glfwPollEvents();
	//Camera view settings
	if (glfwGetKey(m_window, GLFW_KEY_P))
		cam->setPerspective(glm::pi<float>() / 4.f, 16.f / 9.f, 0.1f, 1000.f);
	if (glfwGetKey(m_window, GLFW_KEY_O))
		cam->setOrtho(-15, 15, -15, 15, -11, 50);

#pragma region KeyEvents

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

void CameraApp::draw()
{
	//Draws gizmos objects for camera refrence
	
	glm::mat4 s1 = glm::mat4(1);
	
	Gizmos::addSphere(s1[3], 1, 20, 20, glm::vec4(1.f, 1.f, 0.f, 1.f), &s1);
	Gizmos::addTransform(s1, 1.f);

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

	//draws with what the camera sees
	Gizmos::draw(cam->getProjectionView());
	Gizmos::clear();
}