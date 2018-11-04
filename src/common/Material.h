#pragma once

#include <d3d11.h>
#include "DirectXTK/SimpleMath.h"

class Material
{
public:

	Material() {}
    DirectX::SimpleMath::Vector4 Ambient;
    DirectX::SimpleMath::Vector4 Diffuse;
    DirectX::SimpleMath::Vector4 Specular;
};

