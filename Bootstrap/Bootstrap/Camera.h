#pragma once
#include <vec3.hpp>
#include <mat4x4.hpp>
using namespace glm;

class Camera
{
public:
	Camera();
	virtual ~Camera();
	virtual void update(float deltaTime) = 0;
	static void setPerspective(float fieldOfView, float aspectRatio, float near, float far);
	static void setLookAt(vec3 from, vec3 to, vec3 up);
	void setPosition(vec3 position);
	mat4 getWorldTransform() const;
	mat4 getView() const;
	mat4 getProjection() const;
	mat4 getProjectionView() const;


private:
	mat4 m_worldTransform;
	mat4 m_viewTransform;
	mat4 m_projectionTransform;
	mat4 m_projectionViewTransform;
	static void updateProjectionViewTransform();
};
