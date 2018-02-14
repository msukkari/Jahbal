#pragma once

#include <d3d11.h>
#include "DirectXTK/SimpleMath.h"

using namespace DirectX;

class Camera
{
public:
	Camera();

	Matrix GetLookAtMatrix();
	void UpdatePosition();

	Vector3 m_position;
	Vector3 m_target;

	float m_Phi;
	float m_Theta;
	float m_Radius;
};