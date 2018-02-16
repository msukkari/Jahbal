#pragma once

#include <d3d11.h>
#include <string>

#include "d3dx11effect.h"

class Shader
{
public:
	Shader(ID3D11Device* device);
	~Shader();

	void loadFX(std::string path);

	ID3DX11Effect* m_FX;
	ID3D11InputLayout* m_InputLayout;
	ID3D11Device* m_deviceReference;
};

