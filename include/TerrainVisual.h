#pragma once
#include "VisualComponent.h"

struct TerrainVertex
{
	TerrainVertex(float px, float py, float pz)
		: position(px, py, pz) {}
	TerrainVertex()
		: position(0.0f, 0.0f, 0.0f) {}
	Vector3 position;
};

struct TerrainInfo
{
	TerrainInfo(std::wstring file, UINT w, UINT h, float scale, float cSpacing)
		: heightMapFileName(file), width(w), height(h), heightScale(scale), cellSpacing(cSpacing) {}
	
	std::wstring heightMapFileName;

	UINT width;
	UINT height;

	float heightScale;
	float cellSpacing;
};

class TerrainVisual : public VisualComponent
{
public:
	TerrainVisual(BaseEntity* owner, JRenderer* renderer, TerrainInfo info);
	~TerrainVisual();

	void SetupBuffers();
	void InitHeightMap();

	ID3D11Buffer* m_VB;
	//ID3D11ShaderResourceView* m_diffuseSRV;
	//ID3D11ShaderResourceView* m_specSRV;

	TerrainInfo m_terrainInfo;
	std::vector<float> m_heightMapData;
	std::vector<TerrainVertex> m_vertices;

private:
	void SmoothHeightMap();
	float ComputeHeightAverage(int x, int y);
	boolean isWithinHeightMap(int x , int y);
};