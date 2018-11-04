#pragma once
#include <d3d11.h>
#include "DirectXTK/SimpleMath.h"

class BaseEntity
{
public:
	BaseEntity();
	~BaseEntity();

	virtual void Update(__int64 dt_msec) = 0;

    DirectX::SimpleMath::Vector3 m_position;
};

