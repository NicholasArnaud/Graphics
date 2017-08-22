#pragma once
#include "Application.h"
#include <glm.hpp>


class IntrodutionApp :
	public Application
{
public:
	IntrodutionApp();
	~IntrodutionApp();

	/*
	 runs our application
	 @param title the name of the window
	 @param width the width of the window
	 @param height the height of the window
	 @param fullscreen is teh fullscreen or not
	 */
	
	glm::mat4 m_view;
	glm::mat4 m_projection;
private:

	// Inherited via Application
	virtual void startup() override;
	virtual void shutdown() override;
	virtual void update(float) override;
	virtual void draw() override;
	virtual void run(const char* title, unsigned int width, unsigned int height, bool fullscreen) override;
};

