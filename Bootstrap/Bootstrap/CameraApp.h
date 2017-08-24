#pragma once
#include <vec3.hpp>
#include <mat4x4.hpp>
#include "Application.h"
#include "Transform.h"
using namespace glm;


class CameraApp:
public Application
{
public:
	CameraApp();
	virtual ~CameraApp();
	void setPerspective(float fieldOfView, float aspectRatio, float m_near, float m_far);
	void setOrtho(float m_left, float m_right, float m_bottom, float m_top, float m_near, float m_far);
	void setLookAt(vec3 from, vec3 to, vec3 up);
	void setPosition(vec3 position);
	mat4 getWorldTransform() const;
	mat4 getView() const;
	mat4 getProjection() const;
	mat4 getProjectionView() const;
	float rt;
	float deltaTime;
	float prevTime;
	float currTime;

private:
	CameraApp* cam;
	Transform* m_transform;
	mat4 m_worldTransform;
	mat4 m_viewTransform;
	mat4 m_projectionTransform;
	mat4 m_projectionViewTransform;
	void updateProjectionViewTransform();


	// Inherited via Application
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	void run(const char* title, unsigned int width, unsigned int height, bool fullscreen) override;
};
