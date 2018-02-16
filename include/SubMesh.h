#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

#include "DirectXTK/SimpleMath.h"

using namespace DirectX;

struct Vertex;

class SubMesh
{
public:
	SubMesh(std::vector<Vertex> v, std::vector<int> i);

	void SetupBuffers();

	std::vector<Vertex> m_vertexList;
	std::vector<int> m_indexList;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;
	ID3D11ShaderResourceView* m_SRV;
};