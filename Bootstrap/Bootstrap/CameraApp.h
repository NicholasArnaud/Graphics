#pragma once
#include <vec3.hpp>
#include <mat4x4.hpp>
#include "Application.h"
#include "Camera.h"
struct GLFWwindow;
using namespace glm;

class CameraApp:
public Application
{
public:
	CameraApp();
	virtual ~CameraApp();

	float rt;
	float deltaTime;
	float prevTime;
	float currTime;
	
private:
	Camera* cam;

	// Inherited via Application
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	void run(const char* title, unsigned int width, unsigned int height, bool fullscreen) override;
};
