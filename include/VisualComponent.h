#pragma once

#include <vector>

#include "Component.h"
#include "JRenderer.h"

class Mesh;
class Material;
struct Vertex;

class VisualComponent : public Component
{
public:
	VisualComponent(BaseEntity* owner, JRenderer* renderer);
	~VisualComponent();

	void CreateMesh(std::vector<Vertex> vertexList, std::vector<int> indexList);
	void CreateMaterial();

	ID3D11Device* GetGFXDevice() { return m_Renderer->GetGFXDevice(); }

	Mesh* m_Mesh;
	Material* m_Material;

	JRenderer* m_Renderer;

};

