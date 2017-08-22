#pragma once
#include <glm.hpp>
using namespace glm;
class Camera
{
public:
	Camera();
	~Camera();

	virtual void update(float deltaTime) = 0;
	void setPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void setLookAt(vec3 from, vec3 to, vec3 up);
	void setPosition(vec3 position);
	mat4 getWorldTransform();
	mat4 getView();
	mat4 getProjection();
	mat4 getProjectionView();
	

private:
	 mat4 worldTransform;
	 mat4 viewTransform;
	 mat4 projectionTransform;
	 mat4 projectionViewTransform;
	 void updateProjectionViewTransform();
};

