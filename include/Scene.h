#pragma once

#include <vector>

class Entity;
class Light;
class Camera;

class Scene
{
public:
	void AddEntity(Entity* entity);

	void Shutdown();

	std::vector<Entity*>* GetEntityList() { return &m_EntityList; }
	std::vector<Light*>* GetLightList() { return &m_LightList; }

	void SetActiveCamera(Camera* cam) { m_ActiveCamera = cam; }
	Camera* GetActiveCamera() { return m_ActiveCamera; }

private:
	std::vector<Entity*> m_EntityList;
	std::vector<Light*> m_LightList;
	Camera* m_ActiveCamera;

};
