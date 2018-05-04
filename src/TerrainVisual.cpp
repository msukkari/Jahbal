#include <iostream>
#include <fstream>
#include <algorithm>
#include <DirectXMath.h>

#include "TerrainVisual.h"
#include "BaseEntity.h"
#include "Engine.h"
#include "JRenderer.h"

TerrainVisual::TerrainVisual(BaseEntity* owner, JRenderer* renderer, TerrainInfo info) :
	VisualComponent(owner, renderer, VisualType::TERRAIN), m_terrainInfo(info)
{
	m_numPatchCols = ((info.width - 1) / sCellsPerPatch) + 1;
	m_numPatchRows = ((info.height - 1) / sCellsPerPatch) + 1;
	m_numPatchVertices = m_numPatchCols * m_numPatchRows;
	m_numPatchQuadFaces = (m_numPatchCols - 1) * (m_numPatchRows - 1);

	std::ifstream processedHeightMapFile("resources/textures/terrain_p.raw", std::ios::in | std::ios::binary);

	if (processedHeightMapFile)
	{
		m_heightMapData.resize(m_terrainInfo.height * m_terrainInfo.width, 0);
		
		processedHeightMapFile.read((char*)&m_heightMapData[0], m_heightMapData.size() * sizeof(float));
		processedHeightMapFile.close();
	}
	else
	{
		InitHeightMap();
		SmoothHeightMap();
	}

	InitHeightMapSRV();
	SetupBuffers();
}

TerrainVisual::~TerrainVisual() {}

void TerrainVisual::InitHeightMap()
{
	std::vector<UCHAR> in(m_terrainInfo.width * m_terrainInfo.height);

	std::ifstream heightMapFile;
	heightMapFile.open(m_terrainInfo.heightMapFileName.c_str(), std::ios_base::binary);

	if (heightMapFile)
	{
		heightMapFile.read((char*)&in[0], (std::streamsize)in.size());
		heightMapFile.close();
	}
	else std::cout << "Height map file given to terrain visual is invlid: " << m_terrainInfo.heightMapFileName.c_str() << std::endl;

	m_heightMapData.resize(m_terrainInfo.height * m_terrainInfo.width, 0);
	for (UINT i = 0; i < (m_terrainInfo.height * m_terrainInfo.width); i++)
	{
		m_heightMapData[i] = (in[i] / 255.0f) * m_terrainInfo.heightScale;
	}
}

void TerrainVisual::SmoothHeightMap()
{
	if (m_heightMapData.size() == 0)
	{
		std::cout << "Attempting to smooth heightmap before heightmap data is loaded!" << std::endl;
		return;
	}

	std::ofstream processedHeightMapFile("resources/textures/terrain_p.raw", std::ios::out | std::ios::binary);

	std::vector<float> smoothedData(m_heightMapData.size(), 0);
	for (UINT i = 0; i < m_heightMapData.size(); i++)
	{
		smoothedData[i] = ComputeHeightAverage(i / m_terrainInfo.width, i % m_terrainInfo.width);
	}

	processedHeightMapFile.write((char*)&smoothedData[0], smoothedData.size() * sizeof(float));
	processedHeightMapFile.close();
	m_heightMapData = smoothedData;
}

float TerrainVisual::ComputeHeightAverage(int i, int j)
{
	float sum = 0;
	int count = 0;

	for (int r = i - 1; r <= (i + 1); r++)
	{
		for (int c = j - 1; c <= (j + 1); c++)
		{
			if (isWithinHeightMap(r, c))
			{
				sum += m_heightMapData[(r * m_terrainInfo.width) + c];
				count++;
			}
		}
	}

	return sum / float(count);
}

boolean TerrainVisual::isWithinHeightMap(int i, int j)
{
	return (i >= 0) && (i < m_terrainInfo.height) && (j >= 0) && (j < m_terrainInfo.width);
}

void TerrainVisual::InitHeightMapSRV()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = m_terrainInfo.width;
	texDesc.Height = m_terrainInfo.height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	std::vector<DirectX::PackedVector::HALF> hmap(m_heightMapData.size());
	std::transform(m_heightMapData.begin(), m_heightMapData.end(), hmap.begin(), DirectX::PackedVector::XMConvertFloatToHalf);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
	data.SysMemPitch = m_terrainInfo.width * sizeof(DirectX::PackedVector::HALF);
	data.SysMemSlicePitch = 0;
	ID3D11Texture2D* hmapTex = 0;
	HR(m_Renderer->GetGFXDevice()->CreateTexture2D(&texDesc, &data, &hmapTex));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	HR(m_Renderer->GetGFXDevice()->CreateShaderResourceView(hmapTex, &srvDesc, &m_heightMapSRV));

	ReleaseCOM(hmapTex);
}

void TerrainVisual::SetupBuffers()
{
	InitVB();
	InitIB();
}

void TerrainVisual::InitVB()
{
	std::vector<TerrainVertex> patchVertices(m_numPatchRows * m_numPatchCols);

	float halfWidth = 0.5f * GetWidth();
	float halfDepth = 0.5f * GetDepth();

	float patchWidth = GetWidth() / (m_numPatchCols - 1);
	float patchDepth = GetDepth() / (m_numPatchRows - 1);
	float du = 1.0f / (m_numPatchCols - 1);
	float dv = 1.0f / (m_numPatchRows - 1);

	for (UINT i = 0; i < m_numPatchRows; ++i)
	{
		float z = halfDepth - (i * patchDepth);
		for (UINT j = 0; j < m_numPatchCols; ++j)
		{
			float x = -halfWidth + (j * patchWidth);

			int index = (i * m_numPatchCols) + j;

			patchVertices[index].position = Vector3(x, 0, z);
			patchVertices[index].textureCoord = Vector2(i*du, j*dv);
		}
	}

	for (UINT i = 0; i < m_numPatchRows - 1; ++i)
	{
		for (UINT j = 0; j < m_numPatchCols - 1; ++j)
		{
			UINT patchID = (i * (m_numPatchCols - 1)) + j;
			//patchVertices[(i*m_numPatchCols) + j].boundsY = m_patchBounds[patchID];
		}
	}

	m_vertices = patchVertices;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(TerrainVertex) * patchVertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &patchVertices[0];
	HR(m_Renderer->GetGFXDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));
}

void TerrainVisual::InitIB()
{
	std::vector<USHORT> indices(m_numPatchQuadFaces * 4);

	int cur = 0;
	for (int i = 0; i < (m_numPatchRows - 1); ++i)
	{
		for (int j = 0; j < (m_numPatchCols - 1); ++j)
		{
			int index = (i * m_numPatchCols) + j;

			indices[cur++] = index;
			indices[cur++] = index + 1;
			indices[cur++] = index + m_numPatchCols;
			indices[cur++] = index + m_numPatchCols + 1;
		}
	}

	std::vector<Vector3> vertexPosTest(indices.size());
	for (int i = 0; i < indices.size(); i++)
	{
		vertexPosTest[i] = m_vertices[indices[i]].position;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(USHORT) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Renderer->GetGFXDevice()->CreateBuffer(&ibd, &iinitData, &m_IB));
}