#pragma once

#include <d3d11.h>

#include "BaseEntity.h"
#include "DirectXTK/SimpleMath.h"

class Camera : BaseEntity
{
public:
	Camera(float radius, DirectX::SimpleMath::Vector3 target);
	Camera(float radius);
	Camera();

	void Update(__int64 dt_msec);

    DirectX::SimpleMath::Matrix GetLookAtMatrix();
	void UpdatePosition();

	DirectX::SimpleMath::Vector3 m_position;
    DirectX::SimpleMath::Vector3 m_target;

	float m_Phi;
	float m_Theta;
	float m_Radius;
	int m_moveSpeed;
    int m_scrollSpeed;
};