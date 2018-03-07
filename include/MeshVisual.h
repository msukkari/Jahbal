#pragma once
#include "VisualComponent.h"

class BaseEntity;
class JRenderer;

struct MeshVertex
{
	MeshVertex(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
		: position(px, py, pz), normal(nx, ny, nz), uv(u, v) {}
	MeshVertex() : position(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), uv(0.0f, 0.0f) {}

	Vector3 position;
	Vector3 normal;
	Vector2 uv;
};

class MeshVisual : public VisualComponent
{
public:
	MeshVisual(BaseEntity* owner, JRenderer* renderer);
	~MeshVisual();

	void CreateMesh(std::vector<MeshVertex> vertexList, std::vector<int> indexList);
	void CreateMesh(std::string filename);

	Mesh* m_Mesh;
};