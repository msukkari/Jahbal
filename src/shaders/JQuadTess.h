#pragma once

#include <DirectXMath.h>

#include "core/Shader.h"
#include "DirectXTK/SimpleMath.h"
#include "common/Material.h"
#include "common/Light.h"

class JQuadTess : public Shader
{
public:
	JQuadTess(ID3D11Device* device);
	~JQuadTess();

	WORLD_DEC
	WVP_DEC
	EYEPOS_DEC

	/*
	From Shader:

	ID3DX11EffectTechnique* Tech;
	ID3DX11Effect* m_FX;
	ID3D11InputLayout* m_InputLayout;
	ID3D11Device* m_deviceReference;
	*/
};