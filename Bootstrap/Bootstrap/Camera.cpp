#include "Camera.h"
#include <ext.hpp>
#include <assert.h>
#include <glm.hpp>

Camera::Camera():m_worldTransform(1),m_viewTransform(1),m_projectionTransform(1)
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
	auto x = 1.f / (aspectRatio * tan(fieldOfView / 2.f));
	auto y = 1.f / tan(fieldOfView / 2.f);
	auto z = -1.f *((2.f * far * near) / far - near);
	auto w = -1.f * ((2.f *far * near) / (far - near));
	m_projectionTransform = mat4(
		vec4(x, 0, 0, 0),
		vec4(0, y, 0, 0),
		vec4(0, 0, z, -1.f),
		vec4(0, 0, w, 0));


	/*m_projectionTransform = mat4(1 /(aspectRatio* tan(fieldOfView / 2)), 0, 0, 0,
	                   0, 1 / tan(fieldOfView / 2), 0, 0,
	                   0, 0, -((far +near) / (far - near)),-(2*far* near /(far - near)),
	                   0, 0, -1, 0);
	*/
}

void Camera::setLookAt(vec3 eye, vec3 centre, vec3 up)
{
	vec3  f = eye - centre;
	f = normalize(f);
	vec3 z = f;
	vec3 s = cross(up, z);
	vec3 x = normalize(s);
	vec3 y = cross(z, x);
	mat4 m_view = mat4(x[0], y[0], z[0],0,
		x[1], y[1], z[1],0,
		x[2], y[2], z[2],0,
		0,0,0,1);
	mat4 trans = mat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eye.x, -eye.y, -eye.z, 1);
	m_view = m_view * trans;
	auto m_test = lookAt(eye, centre, up);
	assert(m_view == m_test);
	m_worldTransform = m_view * -1;
	m_projectionViewTransform = m_view *m_projectionTransform ;

}

void Camera::setPosition(vec3 position)
{
	m_viewTransform = translate(m_worldTransform, position);
}


#pragma endregion

void Camera::updateProjectionViewTransform()
{
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}
