#pragma once

#include <vector>

#include "Component.h"
#include "JRenderer.h"

class Mesh;
class Shader;
class Material;
struct Vertex;

class VisualComponent : public Component
{
public:
	VisualComponent(BaseEntity* owner, JRenderer* renderer);
	~VisualComponent();

	void CreateMesh(std::vector<Vertex> vertexList, std::vector<int> indexList);
	void CreateMesh(std::string filename);
	void CreateMaterial();

	ID3D11Device* GetGFXDevice() { return m_Renderer->GetGFXDevice(); }

	Mesh* m_Mesh;
	Shader* m_Shader;
	Material* m_Material;

	JRenderer* m_Renderer;

};

