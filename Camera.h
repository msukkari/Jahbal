#pragma once

#include "SimpleMath.h"


class Camera
{
public:
	Camera();
	Camera(SimpleMath::Vector3 position, SimpleMath::ector3 target);


	SimpleMath::Vector3 m_position;
	SimpleMath::Vector3 m_target;

	SimpleMath::V
};