#pragma once

#include "components/Component.h"

class TransformComponent : public Component
{
public:
	TransformComponent(BaseEntity* owner);
	~TransformComponent();
};

