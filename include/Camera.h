#pragma once

#include <d3d11.h>

#include "BaseEntity.h"
#include "DirectXTK/SimpleMath.h"

using namespace DirectX;

class Camera : BaseEntity
{
public:
	Camera(float radius, Vector3 target);
	Camera(float radius);
	Camera();

	void Update(float dt);

	Matrix GetLookAtMatrix();
	void UpdatePosition();

	Vector3 m_position;
	Vector3 m_target;

	float m_Phi;
	float m_Theta;
	float m_Radius;
	int m_moveSpeed;
};