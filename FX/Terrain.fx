cbuffer cbPerFrame
{
	float4 gTessParams; // [minDist, maxDist, minTess, maxTess]
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gViewProj;
};

SamplerState samHeightMap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VIN
{
	float3 posL		: POSITION;
	float2 tex		: TEXCOORD0;
	float2 boundsY	: TEXCOORD1;
};

struct VOUT
{
	float3 posW		: POSITION;
	float2 tex		: TEXCOORD0;
	float2 boundsY	: TEXCOORD1;
};

struct PatchTess
{
	float EdgeTess[4]	:	SV_TessFactor;
	float InsideTess[2]	:	SV_InsideTessFactor;
};

struct DOUT
{
	float4 posH		: SV_POSITION;
	float3 posW		: POSITION;
	float2 tex		: TEXCOORD0;
	float2 tiledTex	: TEXCOORD1;
};

struct HullOut
{
	float3 posW : POSITION;
	float2 tex	: TEXCOORD0;
};

Texture2D gHeightMap;

VOUT VS(VIN vin)
{
	VOUT vout;

	vout.posW = vin.posL;
	vout.posW.y = gHeightMap.SampleLevel(samHeightMap, vin.tex, 0).r;

	vout.tex = vin.tex;
	vout.boundsY = vin.boundsY;

	return vout;
}

float calcTessFactor(float3 p)
{
	float d = distance(p, gEyePosW);

	float s = saturate((d - gTessParams.x) / (gTessParams.y - gTessParams.x));
	return pow(2, lerp(gTessParams.w, gTessParams.z, s));
}

PatchTess ConstantHS(InputPatch<VOUT, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	float3 e0 = 0.5f * (patch[0].posW + patch[2].posW);
	float3 e1 = 0.5f * (patch[0].posW + patch[1].posW);
	float3 e2 = 0.5f * (patch[1].posW + patch[3].posW);
	float3 e3 = 0.5f * (patch[2].posW + patch[3].posW);
	float3 c = 0.25f * (patch[0].posW + patch[1].posW + patch[2].posW + patch[3].posW);

	pt.EdgeTess[0] = 64; calcTessFactor(e0);
	pt.EdgeTess[1] = 46; calcTessFactor(e1);
	pt.EdgeTess[2] = 64; calcTessFactor(e2);
	pt.EdgeTess[3] = 64; calcTessFactor(e3);

	pt.InsideTess[0] = 64; calcTessFactor(c);
	pt.InsideTess[1] = 64; pt.InsideTess[0];

	return pt;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VOUT, 4> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HullOut hout;

	hout.posW = p[i].posW;
	hout.tex = p[i].tex;

	return hout;
}

[domain("quad")]
DOUT DS(PatchTess patchTess, float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
	DOUT dout;

	dout.posW = lerp(lerp(quad[0].posW, quad[1].posW, uv.x),
		lerp(quad[2].posW, quad[3].posW, uv.x), uv.y);

	dout.tex = lerp(lerp(quad[0].tex, quad[1].tex, uv.x),
		lerp(quad[2].tex, quad[3].tex, uv.x), uv.y);

	dout.tiledTex = dout.tex * 50.0f;

	dout.posW.y = gHeightMap.SampleLevel(samHeightMap, dout.tex, 0).r;

	dout.posH = mul(float4(dout.posW, 1.0f), gViewProj);

	return dout;
}

float PS(DOUT pin) : SV_Target
{
	return float4(1.0f, 0.0f, (129.0f / 255.0f), 1.0f);
}

technique11 Tess
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}