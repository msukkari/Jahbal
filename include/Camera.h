#pragma once

#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;

class Camera
{
public:
	Camera();

	XMMATRIX GetLookAtMatrix();
	void UpdatePosition();

	Vector4 m_position;
	Vector4 m_target;

	float m_Phi;
	float m_Theta;
	float m_Radius;
};