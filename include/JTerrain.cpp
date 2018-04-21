#include "JTerrain.h"
#include "JRenderer.h"

JTerrain::JTerrain(ID3D11Device* device)
	: Shader(device)
{
	loadFX("./FX/Terrain.fxo");
	Tech = m_FX->GetTechniqueByName("Tess");

	// cbPerObject
	ViewProj = m_FX->GetVariableByName("gViewProj")->AsMatrix();

	// cbPerFrame
	EyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();
	TessParams = m_FX->GetVariableByName("gTessParams")->AsVector();

	HeightMap = m_FX->GetVariableByName("gHeightMap")->AsShaderResource();

	D3D11_INPUT_ELEMENT_DESC TerrainVertDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 20,
		D3D11_INPUT_PER_VERTEX_DATA, 0
		},
	};

	D3DX11_PASS_DESC passDesc;
	Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_deviceReference->CreateInputLayout(TerrainVertDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayout));
}

JTerrain::~JTerrain()
{
	ReleaseCOM(ViewProj);
	ReleaseCOM(EyePosW);
	ReleaseCOM(TessParams);
	ReleaseCOM(HeightMap);
}