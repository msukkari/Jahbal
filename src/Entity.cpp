#include "Entity.h"
#include "VisualComponent.h"


Entity::Entity(JRenderer* renderer)
{
	m_VisualComponent = new VisualComponent(this, renderer);
}


Entity::~Entity()
{
}

