#pragma once
#include "Application.h"
#include <glm.hpp>
#include "CameraApp.h"


class IntrodutionApp :
	public Application
{
public:
	IntrodutionApp();
	~IntrodutionApp();

	glm::mat4 m_view;
	glm::mat4 m_projection;
	CameraApp* cam;
private:

	// Inherited via Application
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	void run(const char* title, unsigned int width, unsigned int height, bool fullscreen) override;
};

