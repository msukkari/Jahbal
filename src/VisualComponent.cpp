#include "VisualComponent.h"
#include "Mesh.h"
#include "JRenderer.h"
#include "Material.h"


VisualComponent::VisualComponent(BaseEntity* owner, JRenderer* renderer, VisualType visualType) : 
		Component(owner), m_visualType(visualType)
{
	m_Material = nullptr;

	m_Renderer = renderer;
}


VisualComponent::~VisualComponent()
{
}

void VisualComponent::CreateMaterial()
{
	if (m_Material)
	{
		printf("WARNING: CREATEMATERIAL CALLED WHEN MATERIAL ALREADY EXISTS, MATERIAL IS GOING TO BE OVERWRITTEN");

		delete m_Material;
	}

	//m_Material = new Material();
}