#include "Defs.fx"
#include "LightFuncs.fx"

cbuffer cbPerFrame
{
	DirectionalLight gDLight;
	PointLight gPLight;
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gViewProj;
	Material gMaterial;
};

cbuffer cbFixed
{
	float2 gTexCoords[4] =
	{
		float2(0.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f),
		float2(1.0f, 0.0f)
	};
};

SamplerState BillBoardTextureSampler
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 16;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VIN
{
	float3 posW		: POSITION;
	float2  size		: SIZE;
};

struct VOUT
{
	float3 centerW		: POSITION;
	float2 size			: SIZE;
};

struct GOUT
{
	float4 posH		: SV_POSITION;
	float3 posW		: POSITION;
	float3 normalW	: NORMAL;
	float2 uv		: TEXCOORD;
	uint primID		: SV_PrimitiveID;
};

Texture2D gDiffuseMap;

VOUT VS(VIN vin)
{
	VOUT vout;

	vout.centerW = vin.posW;
	vout.size = vin.size;

	return vout;
}

[maxvertexcount(4)]
void GS(point VOUT gin[1],
	uint primID : SV_PrimitiveID,
	inout TriangleStream<GOUT> triStream)
{
	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 lookat = gEyePosW - gin[0].centerW;
	lookat.y = 0.0f;
	lookat = normalize(lookat);
	float3 right = cross(up, lookat);

	float3 w = right * (gin[0].size.x * 0.5f);
	float3 h = up * (gin[0].size.x * 0.5f);

	float4 v[4];
	v[0] = float4(gin[0].centerW + w - h, 1.0f);
	v[1] = float4(gin[0].centerW + w + h, 1.0f);
	v[2] = float4(gin[0].centerW - w - h, 1.0f);
	v[3] = float4(gin[0].centerW - w + h, 1.0f);

	GOUT gout;
	[unroll]
	for (int i = 0; i < 4; i++)
	{
		gout.posH = mul(v[i], gViewProj);
		gout.posW = v[i].xyz;
		gout.normalW = lookat;
		gout.uv = gTexCoords[i];
		gout.primID = primID;
		triStream.Append(gout);
	}
}

float4 PS(GOUT pin)	: SV_TARGET
{
	float4 mat_d = gDiffuseMap.Sample(BillBoardTextureSampler, pin.uv);
	float4 mat_s = 0;
	float4 mat_a = mat_d;

	clip(mat_d.xyz == float3(0.0f, 0.0f, 0.0f) ? -1.0f : 1.0f);

	pin.normalW = normalize(pin.normalW);
	float3 ptoeye = normalize(gEyePosW - pin.posW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float4 a, d, s;
	computeDirectionalLight(gDLight, pin.normalW, ptoeye, mat_d, mat_s, a, d, s);
	ambient += a;
	diffuse += d;
	spec += s;

	computePointLight(gPLight, pin.normalW, pin.posW, ptoeye, mat_d, mat_s, a, d, s);
	ambient += a;
	diffuse += d;
	spec += s;

	float4 color = ambient + diffuse + spec;
	color.a = gMaterial.ambient.a;

	return color;
}

technique11 Tech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};