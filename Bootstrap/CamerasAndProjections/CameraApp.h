#pragma once

#include "Application.h"
struct Camera;
class CameraApp : public Application
{

	Camera* cam;

	float realTime;
	double deltaTime;
	float prevTime;

public:
	CameraApp();
	virtual ~CameraApp();

	void shutdown() override;
	void startup()override;
	void update(float) override;
	void draw() override;
};

