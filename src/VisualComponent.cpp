#include "VisualComponent.h"
#include "Mesh.h"
#include "Vertex.h"


VisualComponent::VisualComponent()
{
	m_Mesh = nullptr;
	m_Material = nullptr;

}


VisualComponent::~VisualComponent()
{
}

void VisualComponent::CreateMesh(std::vector<Vertex> vertexList)
{
	if (m_Mesh)
	{
		printf("WARNING: CREATEMESH CALLED WHEN MESH ALREADY EXISTS, MESH IS GOING TO BE OVERWRITTEN!");

		m_Mesh->OnDestroy();
		delete m_Mesh;
	}

	m_Mesh = new Mesh(vertexList);
}
