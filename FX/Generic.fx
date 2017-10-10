#include "Defs.fx"

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
};


VOUT VS(VIN vin)
{
	VOUT vout;

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	vout.NormalW = vin.Normal;

	return vout;
}

float4 PS(VOUT pin) : SV_Target
{
	float3 light_direction = gDLight.direction;
	float4 light_ambientC = gDLight.ambient;
	float4 light_diffuseC = gDLight.diffuse;
	float4 light_specC = gDLight.specular;

	float4 material_diffuseC = float4(0.8f, 0.8f, 0.8f, 1.0f);
	float4 material_specC = float4(0.8f, 0.8f, 0.8f, 8.0f);

	/*
	float4 material_ambientC = gMaterial.ambient;
	float4 material_diffuseC = gMaterial.diffuse;
	float4 material_specC = gMaterial.specular;
	*/

	light_direction = normalize(light_direction);
	pin.NormalW = normalize(pin.NormalW);

	float3 ptoeye = gEyePosW - pin.PosW; 
	ptoeye = normalize(ptoeye);

	float3 reflected = reflect(light_direction, pin.NormalW); 
	reflected = normalize(reflected);

	float diffuse_factor = max(dot(-light_direction, pin.NormalW), 0.0f);
	float diffuse_color = diffuse_factor * (material_diffuseC * light_diffuseC);


	float4 spec_color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	if (diffuse_factor > 0.0f)
	{
		float spec_factor = pow(max(dot(reflected, ptoeye), 0.0f), material_specC.w);

		spec_color = spec_factor * (light_specC * material_specC);
	}

	float4 color = diffuse_color + spec_color;

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