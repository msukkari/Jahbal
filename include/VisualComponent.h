#pragma once

#include <vector>

#include "Component.h"
#include "JRenderer.h"

class Mesh;
class Shader;
class Material;

struct MeshVertex;

enum VisualType {MESH, BILLBOARD, TERRAIN};

class VisualComponent : public Component
{
public:

	VisualComponent(BaseEntity* owner, JRenderer* renderer, VisualType visualType);
	~VisualComponent();
	void CreateMaterial();

	ID3D11Device* GetGFXDevice() { return m_Renderer->GetGFXDevice(); }
	
	Shader* m_Shader;
	Material* m_Material;
	JRenderer* m_Renderer;

	VisualType m_visualType;

};

