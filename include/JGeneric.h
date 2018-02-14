#pragma once

#include <DirectXMath.h>

#include "Shader.h"
#include "DirectXTK/SimpleMath.h"
#include "Light.h"
#include "Material.h"

using namespace DirectX;

class JGeneric : public Shader
{
public:
	JGeneric(ID3D11Device* device, std::string filename);
	~JGeneric();

	void SetWorldViewProj(Matrix M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(Matrix M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(Matrix M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const Vector3& v) { EyePosW->SetRawValue(&v, 0, sizeof(Vector3)); }
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