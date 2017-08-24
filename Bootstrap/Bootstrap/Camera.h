#pragma once
#include <vec3.hpp>
#include <mat4x4.hpp>
#include "Application.h"
using namespace glm;


class Camera:
public Application
{
public:
	Camera();
	virtual ~Camera();
	void setPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void setOrtho(float near, float far, float left, float right, float top, float bottom);
	void setLookAt(vec3 from, vec3 to, vec3 up);
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
	void updateProjectionViewTransform();


	// Inherited via Application
	void startup() override;
	void shutdown() override;
	void update(float) override;
	void draw() override;
	void run(const char* title, unsigned int width, unsigned int height, bool fullscreen) override;
};
