#pragma once

#include "BaseEntity.h"
#include "VisualComponent.h"
#include "JRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "DirectXTK/SimpleMath.h"

using namespace DirectX;

class VisualComponent;


class Entity : public BaseEntity
{
public:
	Entity(JRenderer* renderer);
	Entity(JRenderer* renderer, Vector3 position);
	Entity(JRenderer* renderer, Vector3 position, Vector3 rotation);
	~Entity();

	void Update(float dt) {}

	Material* GetMaterial() { return m_VisualComponent->m_Material; }
	Shader* GetShader() { return m_VisualComponent->m_Shader; }

	VisualComponent* m_VisualComponent;

	Vector3 m_rotationEuler;
};

