#pragma once

#include <DirectXMath.h>

#include "core/Shader.h"
#include "DirectXTK/SimpleMath.h"
#include "common/Light.h"
#include "common/Material.h"

class JGeneric : public Shader
{
public:
	JGeneric(ID3D11Device* device);
	~JGeneric();

	WVP_DEC
	WORLD_DEC
	WOLRDINV_DEC
	EYEPOS_DEC
	DLIGHT_DEC
	PLIGHT_DEC
	MAT_DEC

	DIFFUSE_DEC
	SPEC_DEC

	/*
	From Shader:

	ID3DX11EffectTechnique* Tech;
	ID3DX11Effect* m_FX;
	ID3D11InputLayout* m_InputLayout;
	ID3D11Device* m_deviceReference;
	*/
};