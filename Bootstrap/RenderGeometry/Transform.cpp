#include "Transform.h"



Transform::Transform() : m_world(1), m_local(1), m_rotation(1), m_position(1), m_scale(1)
{
}


Transform::~Transform()
{
}

void Transform::rotate(float radians, Axis rotationAxis)
{
	glm::vec3 xaxis = glm::vec3(1, 0, 0);
	glm::vec3 yaxis = glm::vec3(0, 1, 0);
	glm::vec3 zaxis = glm::vec3(0, 0, 1);

	glm::vec3 rotaxis = (rotationAxis == ZAXIS) ? glm::vec3(0, 0, 1)
		: (rotationAxis == YAXIS) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);


	switch (rotationAxis)
	{
	case ZAXIS:
		xaxis = glm::vec3(glm::cos(radians), glm::sin(radians), 0);
		yaxis = glm::vec3(-glm::sin(radians), glm::cos(radians), 0);
		zaxis = glm::vec3(0, 0, 1);
		break;
	case YAXIS:
		xaxis = glm::vec3(cos(radians), 0, -sin(radians));
		yaxis = glm::vec3(0, 1, 0);
		zaxis = glm::vec3(sin(radians), 0, cos(radians));
		break;
	case XAXIS:
		xaxis = glm::vec3(1, 0, 0);
		yaxis = glm::vec3(0, cos(radians), sin(radians));
		zaxis = glm::vec3(0, -sin(radians), cos(radians));
		break;
	default:
		break;
	}

	this->m_rotation = glm::mat4(
		glm::vec4(xaxis, 1),
		glm::vec4(yaxis, 1),
		glm::vec4(zaxis, 1),
		glm::vec4(0, 0, 0, 1));

	m_world = m_rotation;

	glm::mat4 expected = glm::rotate(radians, rotaxis);

	//assert(m_rotation == expected);
}

void Transform::translate(glm::vec3 trans)
{
	glm::mat4 m(1);
	glm::vec4 v(1);
	glm::mat4 translation = glm::mat4(1);
	translation[3].x = trans.x;
	translation[3].y = trans.y;
	translation[3].z = trans.z;
	m_world = m_world * translation;

	glm::mat4 expected = glm::translate(m_world, trans);
}