#pragma once
#include "VisualComponent.h"

struct BillBoardVertex
{
	BillBoardVertex(float px, float py, float pz, float s)
		: position(px, py, pz), size(s){}
	BillBoardVertex(Vector3 pos, float s)
		: position(pos), size(s) {}
	BillBoardVertex() : position(0.0f, 0.0f, 0.0f), size(2.0f) {}

	Vector3 position;
	float size;
};

class BillBoardVisual : public VisualComponent
{
public:
	BillBoardVisual(BaseEntity* owner, JRenderer* renderer, float size);
	~BillBoardVisual();

	void SetupBuffers();

	BillBoardVertex m_vertex;
	ID3D11Buffer* m_VB;
	ID3D11ShaderResourceView* m_diffuseSRV;
	ID3D11ShaderResourceView* m_specSRV;
};