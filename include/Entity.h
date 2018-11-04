#pragma once

#include "BaseEntity.h"
#include "VisualComponent.h"
#include "JRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "DirectXTK/SimpleMath.h"

class VisualComponent;

class Entity : public BaseEntity
{
public:
	Entity(JRenderer* renderer);

	Entity(JRenderer* renderer, DirectX::SimpleMath::Vector3 position);
	Entity(JRenderer* renderer, DirectX::SimpleMath::Vector3 position,
        DirectX::SimpleMath::Vector3 rotation);
	~Entity();

	void Update(__int64 dt_msec) {}

	Material* GetMaterial() { return m_VisualComponent->m_Material; }

	VisualComponent* m_VisualComponent;

    DirectX::SimpleMath::Vector3 m_rotationEuler;
};

