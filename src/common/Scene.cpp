#include "common/Scene.h"
#include "common/Entity.h"
#include "common/Light.h"

void Scene::AddEntity(Entity* entity)
{
	m_EntityList.push_back(entity);
}

void Scene::Shutdown()
{
	for (Entity* e : m_EntityList)
	{
		if(e) delete e->m_VisualComponent;
	}

	for (Light* l : m_LightList)
	{
		if(l) delete l;
	}
}