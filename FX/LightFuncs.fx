#include "Defs.fx"
void computeDirectionalLight(DirectionalLight l, float3 normal, float3 ptoeye, float4 mat_d, float4 mat_s,
							 out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 reflected = reflect(l.direction, normal);
	reflected = normalize(reflected);

	float diffuse_factor = max(dot(-l.direction, normal), 0.0f);
	diffuse = diffuse_factor * (mat_d * l.diffuse);

	if (diffuse_factor > 0.0f)
	{
		float spec_factor = pow(max(dot(reflected, ptoeye), 0.0f), l.specular.w);

		spec = spec_factor * (mat_s * l.specular);
	}

	ambient = mat_d * l.ambient;
}

void computePointLight(PointLight l, float3 normW,  float3 posW, float3 ptoeye, float4 mat_d, float4 mat_s,
					   out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 ptol = l.position - posW;
	float d = length(ptol);

	if (d > l.range) return;

	ptol /= d;

	float att = 1.0f / dot(l.att, float3(1.0f, d, d*d));

	ambient = mat_d * l.ambient;

	float diffuse_factor = max(dot(normW, ptol), 0.0f);
	diffuse = diffuse_factor * att * (mat_d * l.diffuse);

	if (diffuse_factor > 0.0f)
	{
		float3 reflected = reflect(-ptol, normW);
		float spec_factor = pow(max(dot(reflected, ptoeye), 0.0f), l.specular.w);

		spec = spec_factor * att * (mat_s * l.specular);
	}
}