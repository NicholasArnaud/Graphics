#pragma once
#include "Application.h"

struct CameraApp;
class GeometryApplication : public Application
{
public:
	GeometryApplication();
	~GeometryApplication();
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_programID;
	unsigned int m_rows;
	unsigned int m_cols;
	CameraApp* camera;
	
	void generateGrid(unsigned int rows, unsigned int cols);
	void run(const char* title, unsigned width, unsigned height, bool fullscreen) override;
protected:
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
};

