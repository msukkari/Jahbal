#pragma once
#include "VisualComponent.h"

struct BillBoardVertex
{
	BillBoardVertex(float px, float py, float pz, float sx, float sy)
		: position(px, py, pz), size(sx, sy) {}
	BillBoardVertex(Vector3 pos, float sx, float sy)
		: position(pos), size(sx, sy) {}
	BillBoardVertex() : position(0.0f, 0.0f, 0.0f), size(2.0f, 2.0f) {}

	Vector3 position;
	Vector2 size;
};

class BillboardVisual : public VisualComponent
{
public:
	BillboardVisual(BaseEntity* owner, JRenderer* renderer, float sx, float sy);
	~BillboardVisual();

	void SetupBuffers();

	BillBoardVertex m_vertex;
	ID3D11Buffer* m_VB;
	ID3D11ShaderResourceView* m_diffuseSRV;
	ID3D11ShaderResourceView* m_specSRV;
};