#pragma once

#include <d3d11.h>
#include "DirectXTK/SimpleMath.h"

using namespace DirectX;

class Material
{
public:

	Material() {}
	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
};

