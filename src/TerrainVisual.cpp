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

	InitHeightMap();
	SmoothHeightMap();
	InitHeightMapSRV();

	//SetupBuffers();
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

	std::vector<float> smoothedData(m_heightMapData.size(), 0);
	for (UINT i = 0; i < m_heightMapData.size(); i++) smoothedData[i] = ComputeHeightAverage(i / m_terrainInfo.width, i % m_terrainInfo.width);

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

}

void TerrainVisual::InitIB()
{

}