#pragma once
#include <d3d11.h>

#include "DirectXTK/SimpleMath.h"
#include "BaseEntity.h"

using namespace DirectX;

class TransformComponent;

class LightData
{
public:
	Vector4 Ambient;
	Vector4 Diffuse;
	Vector4 Specular;
};

class DLightData : public LightData
{
public:
	DLightData() {}

	Vector3 Direction;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

class PLightData : public LightData
{
public:
	PLightData() {}

	// Packed into 4D vector: (Position, Range)
	Vector3 Position;
	float Range;

	// Packed into 4D vector: (A0, A1, A2, Pad)
	Vector3 Att;
	float Pad; // Pad the last float so we can set an array of lights if we wanted.
};

class SLightData : public LightData
{
public:

	SLightData() {}

	// Packed into 4D vector: (Position, Range)
	Vector3 Position;
	float Range;

	// Packed into 4D vector: (Direction, Spot)
	Vector3 Direction;
	float Spot;

	// Packed into 4D vector: (Att, Pad)
	Vector3 Att;
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
    virtual ~Light();
	LightType m_LightType;

	void Update(__int64 dt_msec) {}

	LightData* m_LightData;
};

