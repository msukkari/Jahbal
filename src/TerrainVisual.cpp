#include "TerrainVisual.h"
#include "BaseEntity.h"
#include "Engine.h"
#include "JRenderer.h"

TerrainVisual::TerrainVisual(BaseEntity* owner, JRenderer* renderer, float sx, float sy) :
	VisualComponent(owner, renderer, VisualType::TERRAIN)
{
	m_vertices = std::vector<TerrainVertex>(4);
	m_vertices[0] = TerrainVertex(-sx, 0.0f, sy);
	m_vertices[1] = TerrainVertex(sx, 0.0f, sy);
	m_vertices[2] = TerrainVertex(-sx, 0.0f, -sy);
	m_vertices[3] = TerrainVertex(sx, 0.0f, -sy);

	SetupBuffers();
}

TerrainVisual::~TerrainVisual() {}

void TerrainVisual::SetupBuffers()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(TerrainVertex) * m_vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_vertices;
	HR(Engine::GetInstance()->GetRenderer()->GetGFXDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));
}

