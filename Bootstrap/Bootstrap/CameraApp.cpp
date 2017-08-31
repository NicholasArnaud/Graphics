#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>

#include <ext.hpp>
#include <assert.h>
#include <glm.hpp>
#include <iostream>

#include "Gizmos.h"
#include "Transform.h"
#include "CameraApp.h"


Transform* sunTransform;
Transform* mercuryTransform;
Transform* venusTransform;
Transform* earthTransform;
CameraApp::CameraApp() :rt(0), deltaTime(0), prevTime(0), currTime(0), cam(nullptr), m_worldTransform(mat4(1)), m_viewTransform(mat4(1)), m_projectionTransform(mat4(1)), m_projectionViewTransform(mat4(1))
{
	m_transform = new Transform();
	setPerspective(pi<float>() / 4.f, 16.f / 9.f, 0.1f, 1000.f);
	//setOrtho(-15, 15, -15, 15, -11, 50);
}


CameraApp::~CameraApp()
{
}


#pragma region getters and setters

mat4 CameraApp::getView() const
{
	return m_viewTransform;
}

mat4 CameraApp::getProjection() const
{
	return m_projectionTransform;
}

mat4 CameraApp::getProjectionView() const
{
	return m_projectionTransform * m_viewTransform;
}

mat4 CameraApp::getWorldTransform() const
{
	return m_worldTransform;
}

void CameraApp::setPerspective(float fieldOfView, float aspectRatio, float m_near, float m_far)
{
	auto x = 1.f / (aspectRatio * tan(fieldOfView / 2.f));
	auto y = 1.f / (tan(fieldOfView / 2.f));
	auto z = -1.f *((m_far + m_near) / (m_far - m_near));
	auto w = -1.f * (2.f *(m_far * m_near) / (m_far - m_near));
	m_projectionTransform = mat4(
		vec4(x, 0, 0, 0),
		vec4(0, y, 0, 0),
		vec4(0, 0, z, -1.f),
		vec4(0, 0, w, 0));
	auto copy = glm::perspective(fieldOfView, aspectRatio, m_near, m_far);
	assert(copy == m_projectionTransform);
}

void CameraApp::setOrtho(float m_left, float m_right, float m_bottom, float m_top, float m_near, float m_far)
{
	vec4 xCol = vec4(2.f / (m_right - m_left), 0, 0, 0);
	vec4 yCol = vec4(0, 2.f / (m_top - m_bottom), 0, 0);
	vec4 zCol = vec4(0, 0, -2.f / (m_far - m_near), 0);
	vec4 wCol = vec4(-1.f*((m_right + m_left) / (m_right - m_left)),
		-1.f*((m_top + m_bottom) / (m_top - m_bottom)),
		-1.f*((m_far + m_near) / (m_far - m_near)), 1);
	m_projectionTransform = mat4(xCol, yCol, zCol, wCol);
	auto copy = ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
	assert(m_projectionTransform == copy);
}

void CameraApp::setLookAt(vec3 eye, vec3 centre, vec3 up)
{
	vec3 z = normalize(eye - centre);
	vec3 s = cross(up, z);
	vec3 x = normalize(s);
	vec3 y = cross(z, x);
	m_viewTransform = mat4(x[0], y[0], z[0], 0,
		x[1], y[1], z[1], 0,
		x[2], y[2], z[2], 0,
		0, 0, 0, 1);
	mat4 trans = mat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eye.x, -eye.y, -eye.z, 1);
	m_viewTransform = m_viewTransform * trans;
	auto m_test = lookAt(eye, centre, up);
	assert(m_viewTransform == m_test);
	m_worldTransform = inverse(m_viewTransform);
}

void CameraApp::setPosition(vec3 position)
{
	m_worldTransform[3] = vec4(position, 1);
	m_transform->rotate(pi<float>() * .25f, ZAXIS);
	m_transform->translate(position);
	m_viewTransform = inverse(m_worldTransform);
}


#pragma endregion

void CameraApp::updateProjectionViewTransform()
{
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}


#pragma region Inherited

void CameraApp::startup()
{
	cam = new CameraApp();
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