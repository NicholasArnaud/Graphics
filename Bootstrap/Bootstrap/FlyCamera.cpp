#include "FlyCamera.h"


/**
 * \brief Constructer
 */
FlyCamera::FlyCamera(): m_speed(0)
{
}


/**
 * \brief Destructer
 */
FlyCamera::~FlyCamera()
{
}


/**
 * \brief 
 * \param speed new speed of the camera
 */
void FlyCamera::setSpeed(float speed)
{
	m_speed = speed;
}

/**
 * \brief 
 * \param deltaTime running time of the program
 */
void FlyCamera::update(float deltaTime)
{
}
