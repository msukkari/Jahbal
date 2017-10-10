#pragma once

#include <DirectXMath.h>

#include "BaseEntity.h"

class TransformComponent;

class LightData
{
};

class DLightData : public LightData
{
public:

	DLightData() {}

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;
	DirectX::XMFLOAT3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

class PLightData : public LightData
{
public:

	PLightData() {}

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;

	// Packed into 4D vector: (Position, Range)
	DirectX::XMFLOAT3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	DirectX::XMFLOAT3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

class SLightData : public LightData
{
public:

	SLightData() {}

	DirectX::XMFLOAT4 Ambient;
	DirectX::XMFLOAT4 Diffuse;
	DirectX::XMFLOAT4 Specular;

	// Packed into 4D vector: (Position, Range)
	DirectX::XMFLOAT3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	DirectX::XMFLOAT3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	DirectX::XMFLOAT3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

enum LightType
{
	Directional = 0,
	Point		= 1,
	Spot		= 2
};

class Light : BaseEntity
{
public:
	Light(LightType lightType);
	LightType m_LightType;

	LightData* m_LightData;
};

