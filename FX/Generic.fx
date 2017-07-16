cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};

struct VIN
{
	float3 PosL    : POSITION;
};

struct VOUT
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
};


VOUT VS(VIN vin)
{
	VOUT vout;

	vout.PosH = float4(vin.PosL, 1.0f);

	return vout;
}

float4 PS(VOUT pin) : SV_Target
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
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