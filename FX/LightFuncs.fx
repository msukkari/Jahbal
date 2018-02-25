#include "Defs.fx"

void computeDirectionalLight(DirectionalLight l, float3 normal, float3 ptoeye, float specPow,
							 out float4 ambient, out float4 diffuse, out float4 spec)
{
	float3 light_direction = l.direction;
	float4 light_ambientC = l.ambient;
	float4 light_diffuseC = l.diffuse;
	float4 light_specC = l.specular;

	float3 reflected = reflect(light_direction, normal);
	reflected = normalize(reflected);

	float diffuse_factor = max(dot(-light_direction, normal), 0.0f);
	diffuse = diffuse_factor * light_diffuseC;


	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
	if (diffuse_factor > 0.0f)
	{
		float spec_factor = pow(max(dot(reflected, ptoeye), 0.0f), specPow);

		spec = spec_factor * light_specC;
	}

	ambient = light_ambientC;
}