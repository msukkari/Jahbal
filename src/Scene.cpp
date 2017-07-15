#include "Scene.h"
#include "Entity.h"

void Scene::AddEntity(Entity* entity)
{
	m_EntityList.push_back(entity);
}

void Scene::Shutdown()
{

}