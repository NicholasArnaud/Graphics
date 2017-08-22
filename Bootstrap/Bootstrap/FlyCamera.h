#pragma once
#include "Camera.h"
class FlyCamera :
	public Camera
{
public:
	FlyCamera();
	~FlyCamera();

	// Inherited via Camera
	virtual void update(float deltaTime) override;
	void setSpeed(float speed);

private:
	float speed;
	vec3 up;
};

