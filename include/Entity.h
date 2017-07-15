#pragma once

#include "BaseEntity.h"

class VisualComponent;

class Entity : BaseEntity
{
public:
	Entity();
	~Entity();

private:
	VisualComponent* m_VisualComponent;
};

