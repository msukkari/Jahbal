cbuffer cbPerFrame
{
	float3 gEyePosW;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
};

struct VIN
{
	float3 posL	: POSITION;
};

struct VOUT
{
	float3 posL	: POSITION;
};

struct PatchTess
{
	float EdgeTess[4]	:	SV_TessFactor;
	float InsideTess[2]	:	SV_InsideTessFactor;
};

struct HullOUT
{
	float3 posL : POSITION;
};

struct DomainOUT
{
	float4 posH : SV_POSITION;
};

VOUT VS(VIN vin)
{
	VOUT vout;
	vout.posL = vin.posL;

	return vout;
}

PatchTess ConstantHS(InputPatch<VOUT, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	float3 centerL = 0.25f * (patch[0].posL + patch[1].posL + patch[2].posL + patch[3].posL);
	float3 centerW = mul(float4(centerL, 1.0f), gWorld).xyz;

	float d = distance(centerW, gEyePosW);

	const float d0 = 20.0f;
	const float d1 = 100.0f;
	float tess = 64.0f * saturate((d1 - d) / (d1 - d0));

	pt.EdgeTess[0] = 4;
	pt.EdgeTess[1] = 4;
	pt.EdgeTess[2] = 4;
	pt.EdgeTess[3] = 4;

	pt.InsideTess[0] = 4;
	pt.InsideTess[1] = 4;

	return pt;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOUT HS(InputPatch<VOUT, 4> p,
	uint i : SV_OutputControlPointID,
	uint patchID : SV_PrimitiveID)
{
	HullOUT hout;
	hout.posL = p[i].posL;

	return hout;
}

[domain("quad")]
DomainOUT DS(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOUT, 4> quad)
{
	DomainOUT dout;

	float3 v1 = lerp(quad[0].posL, quad[1].posL, uv.x);
	float3 v2 = lerp(quad[2].posL, quad[3].posL, uv.x);
	float3 p = lerp(v1, v2, uv.y);

	//p.y = 0.3f * (p.z*sin(p.x) + p.x*cos(p.z));

	dout.posH = mul(float4(p, 1.0f), gWorldViewProj);

	return dout;
}

float4 PS(DomainOUT pin) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

technique11 Tech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetHullShader(CompileShader(hs_5_0, HS()));
		SetDomainShader(CompileShader(ds_5_0, DS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}