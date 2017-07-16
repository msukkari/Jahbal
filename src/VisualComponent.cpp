#include "VisualComponent.h"
#include "Mesh.h"
#include "JRenderer.h"
#include "Material.h"
#include "Shader.h"
#include "Vertex.h"


VisualComponent::VisualComponent(BaseEntity* owner, JRenderer* renderer) : 
		Component(owner)
{

	m_Mesh = nullptr;
	m_Material = nullptr;

	m_Renderer = renderer;
}


VisualComponent::~VisualComponent()
{
}

void VisualComponent::CreateMesh(std::vector<Vertex> vertexList, std::vector<int> indexList)
{
	if (m_Mesh)
	{
		printf("WARNING: CREATEMESH CALLED WHEN MESH ALREADY EXISTS, MESH IS GOING TO BE OVERWRITTEN!");

		m_Mesh->OnDestroy();
		delete m_Mesh;
	}

	m_Mesh = new Mesh(this, vertexList, indexList);
}

void VisualComponent::CreateMaterial()
{
	if (m_Material)
	{
		printf("WARNING: CREATEMATERIAL CALLED WHEN MATERIAL ALREADY EXISTS, MATERIAL IS GOING TO BE OVERWRITTEN");

		delete m_Material;
	}

	m_Material = new Material();
	m_Material->m_Shader = new Shader(this);
}