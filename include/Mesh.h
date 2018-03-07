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

struct MeshVertex;

using namespace DirectX;

struct Texture
{
	unsigned int ID;
	std::string Type;
};

class Mesh
{
public:
	Mesh(VisualComponent* owner, std::vector<MeshVertex> vertexList, std::vector<int> indexList);
	Mesh(VisualComponent* owner, std::string filename);
	~Mesh();

	void OnDestroy();

	std::vector<SubMesh> m_subMeshList;
	std::string m_meshFullPath;
	VisualComponent* m_componentOwner;


private:
	bool createSRVFromAssimpMat(aiMaterial* mat, aiTextureType type, ID3D11ShaderResourceView** srv);
	void processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);
	std::string getFolderFromFullPath(std::string path); //TODO put this in a util class maybe

	std::string m_meshFolder;
};

