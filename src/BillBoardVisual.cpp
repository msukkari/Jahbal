#include "BillBoardVisual.h"
#include "BaseEntity.h"
#include "Engine.h"
#include "JRenderer.h"

BillBoardVisual::BillBoardVisual(BaseEntity* owner, JRenderer* renderer, float size) :
	VisualComponent(owner, renderer, VisualType::BILLBOARD), m_vertex(owner->m_position, size)
{
	SetupBuffers();
}

BillBoardVisual::~BillBoardVisual() {}

void BillBoardVisual::SetupBuffers()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(BillBoardVertex);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_vertex;
	HR(Engine::GetInstance()->GetRenderer()->GetGFXDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));
}

