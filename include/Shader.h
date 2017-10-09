#pragma once

#include <d3d11.h>
#include <string>

#include "d3dx11effect.h"

class Shader
{
public:
	Shader(ID3D11Device* device, const std::string filename);
	~Shader();

	ID3DX11Effect* m_FX;
	ID3D11InputLayout* m_InputLayout;
};

