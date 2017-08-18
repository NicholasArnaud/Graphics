#pragma once
#include "Application.h"
#include <glm.hpp>

class IntrodutionApp :
	public Application
{
public:
	IntrodutionApp();
	~IntrodutionApp();
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	/*
	 runs our application
	 @param title the name of the window
	 @param width the width of the window
	 @param height the height of the window
	 @param fullscreen is teh fullscreen or not
	 */
	void run(const char* title, unsigned int width, unsigned int height, bool fullscreen);
	glm::mat4 m_view;
	glm::mat4 m_projection;
};

