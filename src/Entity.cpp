#include "Entity.h"
#include "VisualComponent.h"


Entity::Entity(JRenderer* renderer) : Entity(renderer, Vector3::Zero, Vector3::Zero) {}

Entity::Entity(JRenderer* renderer, Vector3 position) : Entity(renderer, position, Vector3::Zero){}

Entity::Entity(JRenderer* renderer, Vector3 position, Vector3 rotationAngles)
{
	m_VisualComponent = new VisualComponent(this, renderer);
	m_position = position;
	m_rotationEuler = rotationAngles;
}


Entity::~Entity()
{
	delete m_VisualComponent;
}

