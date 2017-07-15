#pragma once

#include <vector>

class Entity;
class Light;

class Scene
{
public:
	void AddEntity(Entity* entity);

	void Shutdown();

	std::vector<Entity*>* GetEntityList() { return &m_EntityList; }

private:
	std::vector<Entity*> m_EntityList;
	std::vector<Light*> m_LightList;
};
