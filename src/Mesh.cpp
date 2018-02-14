#include <d3d11.h>
#include <iostream>

#include "Mesh.h"
#include "VisualComponent.h"

void processNode(aiNode* node, const aiScene* scene, Mesh* m);

Mesh::Mesh(VisualComponent* owner, std::vector<Vertex> vertexList, std::vector<int> indexList)
{
	m_ComponentOwner = owner;
	m_VertexList = vertexList;
	m_IndexList = indexList;
	m_VertexCount = m_VertexList.size();
	m_IndexCount = m_IndexList.size();


	SetupMesh();
}

Mesh::Mesh(VisualComponent* owner, std::vector<Vertex> vertexList, std::vector<int> indexList, std::vector<Texture> textureList)
{
	m_ComponentOwner = owner;
	m_VertexList = vertexList;
	m_IndexList = indexList;
	m_TextureList = textureList;
	m_VertexCount = m_VertexList.size();
	m_IndexCount = m_IndexList.size();
	m_TextureCount = m_TextureList.size();

	SetupMesh();
}

Mesh::Mesh(VisualComponent* owner, std::string filename)
{
	m_ComponentOwner = owner;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs); 

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene, this);

	SetupMesh();
}


void processMesh(aiMesh* mesh, Mesh* m)
{
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::vector<Texture> textures;

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

	m->m_VertexList.insert(m->m_VertexList.end(), vertices.begin(), vertices.end());
	m->m_VertexCount += vertices.size();

	m->m_IndexList.insert(m->m_IndexList.end(), indices.begin(), indices.end());
	m->m_IndexCount += indices.size();
}

void processNode(aiNode* node, const aiScene* scene, Mesh* m)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, m);
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, m);
	}
}



void Mesh::SetupMesh()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * m_VertexList.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_VertexList[0];
	HR(m_ComponentOwner->GetGFXDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * m_IndexList.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_IndexList[0];
	HR(m_ComponentOwner->GetGFXDevice()->CreateBuffer(&ibd, &iinitData, &m_IB));
}
void Mesh::OnDestroy()
{

}

Mesh::~Mesh()
{
}
