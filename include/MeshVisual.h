#pragma once
#include "VisualComponent.h"

class BaseEntity;
class JRenderer;

class MeshVisual : public VisualComponent
{
public:
	MeshVisual(BaseEntity* owner, JRenderer* renderer);
	~MeshVisual();

	void CreateMesh(std::vector<Vertex> vertexList, std::vector<int> indexList);
	void CreateMesh(std::string filename);

	Mesh* m_Mesh;
};