#include <d3d11.h>

#include "Mesh.h"
#include "VisualComponent.h"


Mesh::Mesh(VisualComponent* owner, std::vector<Vertex> vertexList, std::vector<int> indexList)
{
	m_ComponentOwner = owner;
	m_VertexList = vertexList;
	m_IndexList = indexList;
	m_VertexCount = m_VertexList.size();
	m_IndexCount = m_IndexList.size();


	SetupMesh();
}

Mesh::Mesh(VisualComponent* owner, std::vector<Vertex> vertexList, std::vector<int> indexList, std::vector<Texture> textureList)
{
	m_ComponentOwner = owner;
	m_VertexList = vertexList;
	m_IndexList = indexList;
	m_TextureList = textureList;
	m_VertexCount = m_VertexList.size();
	m_IndexCount = m_IndexList.size();
	m_TextureCount = m_TextureList.size();

	SetupMesh();
}

void Mesh::SetupMesh()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * m_VertexList.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_VertexList[0];
	HR(m_ComponentOwner->GetGFXDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * m_IndexList.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_IndexList[0];
	HR(m_ComponentOwner->GetGFXDevice()->CreateBuffer(&ibd, &iinitData, &m_IB));
}
void Mesh::OnDestroy()
{

}

Mesh::~Mesh()
{
}
