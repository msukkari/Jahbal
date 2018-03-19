#pragma once
#include "VisualComponent.h"

struct TerrainVertex
{
	TerrainVertex(float px, float py, float pz)
		: position(px, py, pz) {}
	TerrainVertex()
		: position(0.0f, 0.0f, 0.0f) {}
	Vector3 position;
};

class TerrainVisual : public VisualComponent
{
public:
	TerrainVisual(BaseEntity* owner, JRenderer* renderer, float sx, float sy);
	~TerrainVisual();

	void SetupBuffers();

	ID3D11Buffer* m_VB;
	//ID3D11ShaderResourceView* m_diffuseSRV;
	//ID3D11ShaderResourceView* m_specSRV;
	std::vector<TerrainVertex> m_vertices;
};