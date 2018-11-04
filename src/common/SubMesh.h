#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

#include "DirectXTK/SimpleMath.h"

struct MeshVertex;

class SubMesh
{
public:
	SubMesh(std::vector<MeshVertex> v, std::vector<int> i);

	void SetupBuffers();

	std::vector<MeshVertex> m_vertexList;
	std::vector<int> m_indexList;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;
	ID3D11ShaderResourceView* m_diffuseSRV;
	ID3D11ShaderResourceView* m_specSRV;
};