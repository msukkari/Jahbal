#include <d3d11.h>
#include <iostream>

#include "core/Engine.h"
#include "common/Mesh.h"
#include "common/SubMesh.h"
#include "components/MeshVisual.h"
#include "core/JRenderer.h"
#include "components/VisualComponent.h"
#include "DirectXTK\WICTextureLoader.h"

Mesh::Mesh(VisualComponent* owner, std::vector<MeshVertex> vertexList, std::vector<int> indexList) :
	m_componentOwner(owner), m_meshFullPath(""), m_meshFolder("")
{
	m_subMeshList.push_back(SubMesh(vertexList, indexList));
}

Mesh::Mesh(VisualComponent* owner, std::string filename) :
	m_componentOwner(owner), m_meshFullPath(filename), m_meshFolder(getFolderFromFullPath(filename))
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
	std::vector<MeshVertex> vertices;
	std::vector<int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		aiVector3D v = mesh->mVertices[i];
		aiVector3D n = mesh->mNormals[i];

		if (mesh->mTextureCoords[0])
		{
			aiVector3D t = mesh->mTextureCoords[0][i];
			vertices.push_back(MeshVertex(v.x, v.y, v.z, n.x, n.y, n.z, t.x, t.y));
		}
		else vertices.push_back(MeshVertex(v.x, v.y, v.z, n.x, n.y, n.z, 0.0f, 0.0f));

	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
	}

	ID3D11ShaderResourceView* diffuseSRV = nullptr;
	ID3D11ShaderResourceView* specSRV = nullptr;
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		createSRVFromAssimpMat(mat, aiTextureType_DIFFUSE, &diffuseSRV);
		createSRVFromAssimpMat(mat, aiTextureType_SPECULAR, &specSRV);
	}

	SubMesh sb(vertices, indices);
	sb.m_diffuseSRV = diffuseSRV;
	sb.m_specSRV = specSRV;
	m_subMeshList.push_back(sb);
}

bool Mesh::createSRVFromAssimpMat(aiMaterial* mat, aiTextureType type, ID3D11ShaderResourceView** srv)
{
	aiString str;
	mat->GetTexture(type, 0, &str);

	// create the shader resource view
	ID3D11Resource* texResource = nullptr;
	std::string path = m_meshFolder + str.C_Str();
	std::wstring wc = std::wstring(path.begin(), path.end());
	DirectX::CreateWICTextureFromFile(
		Engine::GetInstance()->GetRenderer()->GetGFXDevice(),
		Engine::GetInstance()->GetRenderer()->GetGFXDeviceContext(),
		wc.c_str(),
		&texResource, srv);
	ReleaseCOM(texResource);

	return srv == nullptr;
}

std::string Mesh::getFolderFromFullPath(std::string path)
{
	for (int i = path.size(); i >= 0; i--)
	{
		if (path[i] == '/') return path.substr(0, i+1);
	}

	return "";
}

void Mesh::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
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
