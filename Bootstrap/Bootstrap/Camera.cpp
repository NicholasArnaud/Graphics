#include "Camera.h"
#include <ext.hpp>
#include <iostream>

Camera::Camera()
{
}


Camera::~Camera()
{
}


#pragma region getters and setters

mat4 Camera::getView() const
{
	return m_viewTransform;
}

mat4 Camera::getProjection() const
{
	return m_projectionTransform;
}

mat4 Camera::getProjectionView() const
{
	return m_projectionTransform * m_viewTransform;
}

mat4 Camera::getWorldTransform() const
{
	return m_worldTransform;
}

void Camera::setPerspective(float fieldOfView, float aspectRatio, float near, float far)
{
	mat4 scalar = mat4(1 /(aspectRatio* tan(fieldOfView / 2)), 0, 0, 0,
	                   0, 1 / tan(fieldOfView / 2), 0, 0,
	                   0, 0, -((far +near) / (far - near)),-(2*far* near /(far - near)),
	                   0, 0, -1, 0);
	
}

void Camera::setLookAt(vec3 from, vec3 to, vec3 up)
{
}

void Camera::setPosition(vec3 position)
{
	m_worldTransform = translate(m_worldTransform, position);
}


#pragma endregion

void Camera::updateProjectionViewTransform()
{
}
