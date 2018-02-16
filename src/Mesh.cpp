#include <d3d11.h>
#include <iostream>

#include "Engine.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "VisualComponent.h"
#include "DirectXTK\WICTextureLoader.h"

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


void Mesh::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D v = mesh->mVertices[i];
		aiVector3D n = mesh->mNormals[i];

		if (mesh->mTextureCoords[0])
		{
			aiVector3D t = mesh->mTextureCoords[0][i];
			vertices.push_back(Vertex(v.x, v.y, v.z, n.x, n.y, n.z, t.x, t.y));
		}
		else vertices.push_back(Vertex(v.x, v.y, v.z, n.x, n.y, n.z, 0.0f, 0.0f));

	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
	}

	ID3D11ShaderResourceView* srv = nullptr;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);

		ID3D11Resource* texResource = nullptr;
		std::string s = str.C_Str();
		std::string objectName;
		if (s.find('_') == std::string::npos) objectName = s.substr(0, s.find('.'));
		else objectName = s.substr(0, s.find('_'));

		s = "resources/objects/" + objectName + '/' + s;
		
		std::wstring wc = std::wstring(s.begin(), s.end());
		CreateWICTextureFromFile(
			Engine::GetInstance()->GetRenderer()->GetGFXDevice(),
			Engine::GetInstance()->GetRenderer()->GetGFXDeviceContext(),
			wc.c_str(), 
			&texResource, &srv);
		ReleaseCOM(texResource);
	}

	SubMesh sb(vertices, indices);
	sb.m_SRV = srv;
	m_subMeshList.push_back(sb);
}

void Mesh::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
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
