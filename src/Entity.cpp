#include "Entity.h"
#include "VisualComponent.h"


Entity::Entity(JRenderer* renderer) : Entity(renderer, Vector3::Zero) {}

Entity::Entity(JRenderer* renderer, Vector3 position)
{
	m_VisualComponent = new VisualComponent(this, renderer);
	m_position = position;
}


Entity::~Entity()
{
	delete m_VisualComponent;
}

