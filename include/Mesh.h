#pragma once

#include <vector>
#include <DirectXMath.h>

#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<Vertex> vertexList);
	~Mesh();

	void OnDestroy();

private:
	std::vector<Vertex> m_VertexList;
};

