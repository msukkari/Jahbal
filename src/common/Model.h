#pragma once
#include <vector>
#include "common/Mesh.h"

class aiNode;
class aiMaterial;
class aiMesh;
class aiScene;
struct aiTextureType;

class Model
{
public:
	Model(char* path) { LoadModel(path); }

	//void Draw ??

private:
	std::vector<Mesh> m_MeshList;
	std::string m_Directory;

	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	//Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	//std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};