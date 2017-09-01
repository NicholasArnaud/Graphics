#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

#include <ext.hpp>
#include <assert.h>
#include <glm.hpp>
#include <iostream>

#include "Gizmos.h"
#include "CameraApp.h"

Transform* sunTransform;
Transform* mercuryTransform;
Transform* venusTransform;
Transform* earthTransform;
CameraApp::CameraApp() :rt(0), deltaTime(0), prevTime(0), currTime(0), cam(nullptr)
{
	cam->setPerspective(pi<float>() / 4.f, 16.f / 9.f, 0.1f, 1000.f);
	//setOrtho(-15, 15, -15, 15, -11, 50);
}


CameraApp::~CameraApp()
{
}

#pragma region Inherited

void CameraApp::startup()
{
	cam = new Camera();
	sunTransform = new Transform();
	mercuryTransform = new Transform();
	venusTransform = new Transform();
	earthTransform = new Transform();
	cam->setLookAt(vec3(10, 10, 10), vec3(0, 0, 0), vec3(0, 1, 0));

}

void CameraApp::shutdown()
{
}

void CameraApp::update(float deltaTime)
{
	rt += deltaTime;
	sunTransform->m_world = mat4(1);
	sunTransform->rotate(rt *.1f, YAXIS);
	mercuryTransform->m_world = rotate(rt, vec3(0, 1, 0))*sunTransform->m_world* translate(vec3(0, 0, 5))* rotate(rt*.5f, vec3(0, 1, 0));
	venusTransform->m_world = sunTransform->m_world * translate(vec3(0, 0, 7)) * rotate(rt*.3f, vec3(0, 1, 0));
	earthTransform->m_world = sunTransform->m_world * translate(vec3(0, 0, 8)) * rotate(rt*.28f, vec3(0, 1, 0));

	if (glfwGetKey(m_window, GLFW_KEY_W))
		cam->setPosition(cam->getWorldTransform()[3] -= cam->getWorldTransform()[2] * .2f);
	if (glfwGetKey(m_window, GLFW_KEY_S))
		cam->setPosition(cam->getWorldTransform()[3] += cam->getWorldTransform()[2] * .2f);
	if (glfwGetKey(m_window, GLFW_KEY_A))
		cam->setPosition(cam->getWorldTransform()[3] -= cam->getWorldTransform()[0] * .2f);
	if (glfwGetKey(m_window, GLFW_KEY_D))
		cam->setPosition(cam->getWorldTransform()[3] += cam->getWorldTransform()[0] * .2f);
	if(glfwGetKey(m_window, GLFW_KEY_P))
	{
		cam->setPerspective(pi<float>() / 4.f, 16.f / 9.f, 0.1f, 1000.f);
	}
		
	if (glfwGetKey(m_window, GLFW_KEY_O))
	{
		cam->setOrtho(-15, 15, -15, 15, -11, 50);
	}
		

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

		mat4 rEle = rotate((float)iDeltaX * 1 / 800, vec3(0, 1, 0));
		mat4 rAzi = rotate((float)iDeltaY * 1 / 800, vec3(1, 0, 0));
		cam->m_worldTransform = rEle* rAzi * cam->m_worldTransform;
		cam->m_viewTransform = inverse(cam->m_worldTransform);
		std::cout << "delta mouse:: " << to_string(vec2(iDeltaX, iDeltaY)) << std::endl;
	}
}

void CameraApp::draw()
{
	mat4 s1 = mat4(sunTransform->m_world);
	mat4 s2 = mat4(mercuryTransform->m_world);
	mat4 s3 = mat4(venusTransform->m_world);
	mat4 s4 = mat4(earthTransform->m_world);
	vec4 center = vec4(0, 5, 0, 1);
	vec4 color = vec4(0, 0, 0, 0);
	Gizmos::clear();
	Gizmos::addSphere(sunTransform->m_world[3], 4.32288f, 20, 20, vec4(1.f, 1.f, 0.f, 1.f), &sunTransform->m_world);
	Gizmos::addSphere(mercuryTransform->m_world[3], .02439f, 20, 20, vec4(1.f, .5f, .1f, 1.f), &mercuryTransform->m_world);
	Gizmos::addSphere(venusTransform->m_world[3], .06378f, 20, 20, vec4(1.2f, .5f, .1f, 1.f), &venusTransform->m_world);
	Gizmos::addSphere(earthTransform->m_world[3], .06378f, 20, 20, vec4(0, 0, 1, 0), &earthTransform->m_world);
	Gizmos::addTransform(s1, 1.5f);
	Gizmos::addTransform(s2, .5f);
	Gizmos::addTransform(s3, .5f);
	Gizmos::addTransform(s4, .5f);
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
	update(deltaTime);
	Gizmos::draw(cam->getProjectionView());
}

void CameraApp::run(const char * title, unsigned int width, unsigned int height, bool fullscreen)
{
	startup();
	if (glfwInit() == GL_FALSE)
		return;

	GLFWmonitor* monitor = (fullscreen) ? glfwGetPrimaryMonitor() : nullptr;
	m_window = glfwCreateWindow(width, height, title, monitor, nullptr);

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return;
	}
	Gizmos::create();
	glClearColor(0.5f, 0.5f, 0.5f, 1);

	glEnable(GL_DEPTH_TEST);


	while (!m_gameover)
	{
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;
		prevTime = currTime;
		glfwPollEvents();





		update(deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		draw();



		glfwSwapBuffers(m_window);
		m_gameover = (glfwWindowShouldClose(m_window) == GLFW_TRUE);
	}
}
#pragma  endregion 