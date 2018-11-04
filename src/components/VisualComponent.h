#pragma once

#include <vector>

#include "components/Component.h"
#include "core/JRenderer.h"

class Mesh;
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
	
	Material* m_Material;
	JRenderer* m_Renderer;

	VisualType m_visualType;

};

