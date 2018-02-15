#include <d3d11.h>
#include <iostream>

#include "Engine.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "VisualComponent.h"

Mesh::Mesh(VisualComponent* owner, std::vector<Vertex> vertexList, std::vector<int> indexList) :
	m_componentOwner(owner)
{
	m_subMeshList.push_back(SubMesh(vertexList, indexList));
}

Mesh::Mesh(VisualComponent* owner, std::vector<Vertex> vertexList, std::vector<int> indexList, std::vector<Texture> textureList) :
	m_componentOwner(owner)
{
}

Mesh::Mesh(VisualComponent* owner, std::string filename) :
	m_componentOwner(owner)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs); 

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}


void Mesh::processMesh(aiMesh* mesh)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D v = mesh->mVertices[i];
		aiVector3D n = mesh->mNormals[i];
		
		vertices.push_back(Vertex(v.x, v.y, v.z, n.x, n.y, n.z, 0.0f, 0.0f));
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
	}

	SubMesh sb(vertices, indices);
	m_subMeshList.push_back(sb);
}

void Mesh::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


void Mesh::OnDestroy()
{

}

Mesh::~Mesh()
{
}
