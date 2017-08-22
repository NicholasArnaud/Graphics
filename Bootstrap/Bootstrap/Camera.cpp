#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}


#pragma region getters and setters

mat4 Camera::getView()
{
	return viewTransform;
}

mat4 Camera::getProjection()
{
	return projectionTransform;
}

mat4 Camera::getProjectionView()
{
	return projectionViewTransform;
}

mat4 Camera::getWorldTransform()
{
	return worldTransform;
}

void Camera::setPerspective(float fieldOfView, float aspectRatio, float near, float)
{
}

void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
}

void Camera::setPosition(vec3 position)
{
}



#pragma endregion

void Camera::updateProjectionViewTransform()
{

}
