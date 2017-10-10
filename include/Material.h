#pragma once

#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;

class Material
{
public:

	Material() {}
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
};

