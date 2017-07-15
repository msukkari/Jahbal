#pragma once

#include <vector>

#include "Component.h"

class Mesh;
class Material;
struct Vertex;

class VisualComponent : public Component
{
public:
	VisualComponent();
	~VisualComponent();

	void CreateMesh(std::vector<Vertex> vertexList);

private:
	Mesh* m_Mesh;
	Material* m_Material;
};

