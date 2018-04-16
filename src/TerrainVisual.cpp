#include <iostream>
#include <fstream>

#include "TerrainVisual.h"
#include "BaseEntity.h"
#include "Engine.h"
#include "JRenderer.h"

TerrainVisual::TerrainVisual(BaseEntity* owner, JRenderer* renderer, TerrainInfo info) :
	VisualComponent(owner, renderer, VisualType::TERRAIN), m_terrainInfo(info)
{
	InitHeightMap();
	SmoothHeightMap();

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

void TerrainVisual::SetupBuffers()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(TerrainVertex) * 4;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_vertices[0];
	HR(Engine::GetInstance()->GetRenderer()->GetGFXDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));
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

float TerrainVisual::ComputeHeightAverage(int x, int y)
{
	float sum = 0;
	int count = 0;

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (isWithinHeightMap(x + i, y + j))
			{
				sum += m_heightMapData[(x + i) + ((y + j) * m_terrainInfo.width)];
				count++;
			}
		}
	}

	return sum / float(count);
}

boolean TerrainVisual::isWithinHeightMap(int x, int y)
{
	return (x >= 0) && (x < m_terrainInfo.width) && (y >= 0) && (y < m_terrainInfo.height);
}

