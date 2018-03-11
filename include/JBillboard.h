#pragma once

#include "Shader.h"
#include "DirectXTK\SimpleMath.h"
#include "Light.h"
#include "Material.h"

using namespace DirectX;

class JBillboard : public Shader
{
public: 
	JBillboard(ID3D11Device* device);
	~JBillboard();

	VP_DEC
	MAT_DEC
	EYEPOS_DEC
	DLIGHT_DEC
	PLIGHT_DEC

	DIFFUSE_DEC

	/*
	From Shader:

	ID3DX11EffectTechnique* Tech;
	ID3DX11Effect* m_FX;
	ID3D11InputLayout* m_InputLayout;
	ID3D11Device* m_deviceReference;
	*/
};