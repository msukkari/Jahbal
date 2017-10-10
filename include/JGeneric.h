#pragma once

#include <DirectXMath.h>

#include "Shader.h"
#include "SimpleMath.h"
#include "Light.h"
#include "Material.h"

using namespace DirectX;

class JGeneric : public Shader
{
public:
	JGeneric(ID3D11Device* device, std::string filename);
	~JGeneric();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDLight(const LightData* data) { DirectionalLight->SetRawValue(data, 0, sizeof(DLightData)); }
	void SetMaterial(const Material* mat) { Mat->SetRawValue(mat, 0, sizeof(Material)); }

	ID3DX11EffectTechnique* Tech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVariable* DirectionalLight;
	ID3DX11EffectVariable* Mat;

};