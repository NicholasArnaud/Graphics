#pragma once
#include "Camera.h"

class FlyCamera :
	public Camera
{
public:
	FlyCamera();
	~FlyCamera();

	// Inherited via Camera
	void update(float deltaTime) override;
	void setSpeed(float speed);

private:
	float m_speed;
	vec3 m_up;
};
