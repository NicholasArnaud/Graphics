#pragma once
#include <mat4x2.hpp>
#include "Transform.h"

class Camera
{
public:
	Camera();
	~Camera();

	Transform* m_transform;

	glm::mat4 getWorldTransform() const;
	glm::mat4 getView() const;
	glm::mat4 getProjection() const;
	glm::mat4 getProjectionView() const;


	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTransform;
	glm::mat4 m_positionTransform;
	void updateProjectionViewTransform();
	void setLookAt(glm::vec3 eye, glm::vec3 centre, glm::vec3 up);
	void setPosition(glm::vec3 position);
	void setPerspective(float fieldOfView, float aspectRatio, float m_near, float m_far);
	void setOrtho(float m_left, float m_right, float m_bottom, float m_top, float m_near, float m_far);
};

