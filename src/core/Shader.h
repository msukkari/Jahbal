#pragma once

#include <d3d11.h>
#include <string>

#include "util/d3dx11effect.h"

class Shader
{
public:
	Shader(ID3D11Device* device);
	~Shader();

	void loadFX(std::string path);

	ID3DX11EffectTechnique* Tech;
	ID3DX11Effect* m_FX;
	ID3D11InputLayout* m_InputLayout;
	ID3D11Device* m_deviceReference;
};


#define WVP_DEC	\
	void SetWorldViewProj(DirectX::SimpleMath::Matrix M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }	\
	ID3DX11EffectMatrixVariable* WorldViewProj;	\

#define WORLD_DEC	\
	void SetWorld(DirectX::SimpleMath::Matrix M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }	\
	ID3DX11EffectMatrixVariable* World;	\

#define WOLRDINV_DEC	\
	void SetWorldInvTranspose(DirectX::SimpleMath::Matrix M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }	\
	ID3DX11EffectMatrixVariable* WorldInvTranspose;	\

#define TPARAMS_DEC \
	void SetTessParams(const DirectX::SimpleMath::Vector4& v) {TessParams->SetRawValue(&v, 0, sizeof(DirectX::SimpleMath::Vector4));} \
	ID3DX11EffectVectorVariable* TessParams; \

#define VP_DEC \
	void SetViewProj(DirectX::SimpleMath::Matrix M) {ViewProj->SetMatrix(reinterpret_cast<const float*>(&M));}	\
	ID3DX11EffectMatrixVariable* ViewProj;	\

#define EYEPOS_DEC																				\
	void SetEyePosW(const DirectX::SimpleMath::Vector3& v) { EyePosW->SetRawValue(&v, 0, sizeof(DirectX::SimpleMath::Vector3)); }			\
	ID3DX11EffectVectorVariable* EyePosW;															\

#define DLIGHT_DEC	\
	void SetDLight(const LightData* data) { DirectionalLight->SetRawValue(data, 0, sizeof(DLightData)); }	\
	ID3DX11EffectVariable* DirectionalLight;	\

#define PLIGHT_DEC	\
	void SetPLight(const LightData* data) { PointLight->SetRawValue(data, 0, sizeof(PLightData)); }	\
	ID3DX11EffectVariable* PointLight;	\

#define MAT_DEC	\
	void SetMaterial(const Material* mat) { Mat->SetRawValue(mat, 0, sizeof(Material)); }	\
	ID3DX11EffectVariable* Mat;	\

#define DIFFUSE_DEC	\
	void SetDiffuseMap(ID3D11ShaderResourceView* d) { if (d) DiffuseMap->SetResource(d); }	\
	ID3DX11EffectShaderResourceVariable* DiffuseMap;	\

#define SPEC_DEC	\
	void SetSpecMap(ID3D11ShaderResourceView* s) { if (s) SpecMap->SetResource(s); } \
	ID3DX11EffectShaderResourceVariable* SpecMap;	\

#define HMAP_DEC \
	void SetHeightMap(ID3D11ShaderResourceView* h) { if(h) HeightMap->SetResource(h); } \
	ID3DX11EffectShaderResourceVariable* HeightMap; \





