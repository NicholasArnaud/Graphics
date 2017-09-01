#pragma once
#include <ext.hpp>

enum Axis
{
	XAXIS,
	YAXIS,
	ZAXIS,
};

class Transform
{
public:
	Transform();
	~Transform();
	glm::mat4 m_world;
	glm::mat4 m_local;
	glm::mat4 m_rotation;
	glm::vec3 m_position;
	float m_scale;
	void rotate(float radians, Axis rotationAxis);
	void translate(glm::vec3 trans);

};