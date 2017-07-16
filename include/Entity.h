#pragma once

#include "BaseEntity.h"
#include "VisualComponent.h"
#include "JRenderer.h"

class VisualComponent;

class Entity : BaseEntity
{
public:
	Entity(JRenderer* renderer);
	~Entity();

	VisualComponent* m_VisualComponent;
};

