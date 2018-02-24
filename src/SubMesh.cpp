#include <d3d11.h>

#include "Engine.h"
#include "JRenderer.h"
#include "Mesh.h"
#include "SubMesh.h"

SubMesh::SubMesh(std::vector<Vertex> v, std::vector<int> i) :
	m_vertexList(v), m_indexList(i), m_diffuseSRV(0), m_specSRV(0)
{
	SetupBuffers();
}

void SubMesh::SetupBuffers()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * m_vertexList.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_vertexList[0];
	HR(Engine::GetInstance()->GetRenderer()->GetGFXDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * m_indexList.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_indexList[0];
	HR(Engine::GetInstance()->GetRenderer()->GetGFXDevice()->CreateBuffer(&ibd, &iinitData, &m_IB));
}