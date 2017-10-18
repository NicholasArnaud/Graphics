#include "Camera.h"



Camera::Camera(): m_transform(), m_worldTransform(glm::mat4(1)), m_viewTransform(glm::mat4(1)), m_projectionTransform(glm::mat4(1)), m_projectionViewTransform(glm::mat4(1))
{
}


Camera::~Camera()
{
}

glm::mat4 Camera::getView() const
{
	return m_viewTransform;
}

glm::mat4 Camera::getProjection() const
{
	return m_projectionTransform;
}

glm::mat4 Camera::getProjectionView() const
{
	return m_projectionTransform * m_viewTransform;
}

glm::mat4 Camera::getWorldTransform() const
{
	return m_worldTransform;
}

void Camera::updateProjectionViewTransform()
{
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}

void Camera::setLookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 up)
{
	glm::vec3 z = glm::normalize(eye - centre);
	glm::vec3 s = glm::cross(up, z);
	glm::vec3 x = glm::normalize(s);
	glm::vec3 y = glm::cross(z, x);
	m_viewTransform = glm::mat4(x[0], y[0], z[0], 0,
		x[1], y[1], z[1], 0,
		x[2], y[2], z[2], 0,
		0, 0, 0, 1);
	glm::mat4 trans = glm::mat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eye.x, -eye.y, -eye.z, 1);
	m_viewTransform = m_viewTransform * trans;
	auto m_test = glm::lookAt(eye, centre, up);
	assert(m_viewTransform == m_test);
	m_worldTransform = glm::inverse(m_viewTransform);
}

void Camera::setPosition(glm::vec3 position)
{
	m_worldTransform[3] = glm::vec4(position, 1);
	m_viewTransform = glm::inverse(m_worldTransform);
}


void Camera::setPerspective(float fieldOfView, float aspectRatio, float m_near, float m_far)
{
	auto x = 1.f / (aspectRatio * tan(fieldOfView / 2.f));
	auto y = 1.f / (tan(fieldOfView / 2.f));
	auto z = -1.f *((m_far + m_near) / (m_far - m_near));
	auto w = -1.f * (2.f *(m_far * m_near) / (m_far - m_near));
	m_projectionTransform = glm::mat4(
		glm::vec4(x, 0, 0, 0),
		glm::vec4(0, y, 0, 0),
		glm::vec4(0, 0, z, -1.f),
		glm::vec4(0, 0, w, 0));
	auto copy = glm::perspective(fieldOfView, aspectRatio, m_near, m_far);
	assert(copy == m_projectionTransform);
}

void Camera::setOrtho(float m_left, float m_right, float m_bottom, float m_top, float m_near, float m_far)
{
	glm::vec4 xCol = glm::vec4(2.f / (m_right - m_left), 0, 0, 0);
	glm::vec4 yCol = glm::vec4(0, 2.f / (m_top - m_bottom), 0, 0);
	glm::vec4 zCol = glm::vec4(0, 0, -2.f / (m_far - m_near), 0);
	glm::vec4 wCol = glm::vec4(-1.f*((m_right + m_left) / (m_right - m_left)),
		-1.f*((m_top + m_bottom) / (m_top - m_bottom)),
		-1.f*((m_far + m_near) / (m_far - m_near)), 1);
	m_projectionTransform = glm::mat4(xCol, yCol, zCol, wCol);
	auto copy = glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
	assert(m_projectionTransform == copy);
}