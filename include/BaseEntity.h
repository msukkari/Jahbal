#pragma once
#include <d3d11.h>
#include "DirectXTK/SimpleMath.h"

using namespace DirectX;

class BaseEntity
{
public:
	BaseEntity();
	~BaseEntity();

	virtual void Update(float dt) = 0;

	Vector3 m_position;
};

