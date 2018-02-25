#include "Defs.fx"
#include "LightFuncs.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDLight;
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
};

cbuffer cbPerObject2
{
	Material gMaterial;
};

struct VIN
{
	float3 PosL     : POSITION;
	float3 Normal   : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct VOUT
{
	float4 PosH      : SV_POSITION;
	float4 PosW      : POSITION;
	float3 NormalW   : NORMAL;
	float2 UV		 : TEXCOORD;
};

SamplerState MeshTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

Texture2D gDiffuseMap;
Texture2D gSpecMap;

VOUT VS(VIN vin)
{
	VOUT vout;

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.NormalW = mul(vin.Normal, (float3x3)gWorldInvTranspose);
	vout.UV = vin.TexCoord;

	return vout;
}

float4 PS(VOUT pin) : SV_Target
{
	float4 material_diffuseC = gDiffuseMap.Sample(MeshTextureSampler, pin.UV);// gMaterial.diffuse;
	float4 material_specC = gSpecMap.Sample(MeshTextureSampler, pin.UV); // gMaterial.specular;
	float4 material_ambientC = material_diffuseC;
	
	pin.NormalW = normalize(pin.NormalW);
	float3 ptoeye = normalize(gEyePosW - pin.PosW); 

	float4 dirLight_ambientC = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 dirLight_diffuseC = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 dirLight_specC = float4(0.0f, 0.0f, 0.0f, 0.0f);
	computeDirectionalLight(gDLight, pin.NormalW, ptoeye, material_specC.w, dirLight_ambientC, dirLight_diffuseC, dirLight_specC);

	float4 color = (dirLight_ambientC * material_ambientC) +
				   (dirLight_diffuseC * material_diffuseC) + 
				   (dirLight_specC * material_specC);
	color.a = gMaterial.ambient.a;

	return color;
}

technique11 Tech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};