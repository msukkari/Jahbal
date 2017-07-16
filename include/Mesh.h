#pragma once

#include <vector>
#include <DirectXMath.h>

#include "Vertex.h"

class VisualComponent;

class Mesh
{
public:
	Mesh(VisualComponent* owner, std::vector<Vertex> vertexList, std::vector<int> indexList);
	~Mesh();

	void OnDestroy();

	std::vector<Vertex> m_VertexList;
	std::vector<int> m_IndexList;
	int m_IndexCount;
	int m_VertexCount;

	VisualComponent* m_ComponentOwner;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

};

