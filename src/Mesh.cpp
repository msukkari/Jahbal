#include "Mesh.h"



Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<Vertex> vertexList)
{
	m_VertexList = vertexList;
}

void Mesh::OnDestroy()
{

}

Mesh::~Mesh()
{
}
