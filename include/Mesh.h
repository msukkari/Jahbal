#pragma once

#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "DirectXTK/SimpleMath.h"

class VisualComponent;
class SubMesh;

using namespace DirectX;

struct VertexPN
{
	Vector3 Position;
	Vector3 Normal;
};

// pos, normal and text coords
struct Vertex
{
	Vertex(float px, float py, float pz, float nx, float ny, float nz, float u, float v)
		: Position(px, py, pz), Normal(nx, ny, nz), UV(u, v) {}
	Vertex() : Position(0.0f, 0.0f, 0.0f), Normal(0.0f, 0.0f, 0.0f), UV(0.0f, 0.0f) {}

	Vector3 Position;
	Vector3 Normal;
	Vector2 UV;
};

struct Texture
{
	unsigned int ID;
	std::string Type;
};

class Mesh
{
public:
	Mesh(VisualComponent* owner, std::vector<Vertex> vertexList, std::vector<int> indexList);
	Mesh(VisualComponent* owner, std::vector<Vertex> vertexList, std::vector<int> indexList, std::vector<Texture> textureList);
	Mesh(VisualComponent* owner, std::string filename);
	~Mesh();

	void OnDestroy();

	std::vector<SubMesh> m_subMeshList;
	std::string m_meshFilePath;
	VisualComponent* m_componentOwner;


private:
	void processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);
};

