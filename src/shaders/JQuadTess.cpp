#include "shaders/JQuadTess.h"
#include "core/JRenderer.h"

JQuadTess::JQuadTess(ID3D11Device* device)
	: Shader(device)
{
	loadFX("./FX/QuadTess.fxo");
	Tech = m_FX->GetTechniqueByName("Tess");

	// cbPerObject
	WorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = m_FX->GetVariableByName("gWorld")->AsMatrix();

	// cbPerFrame
	EyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();

	D3D11_INPUT_ELEMENT_DESC PosDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3DX11_PASS_DESC passDesc;
	Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_deviceReference->CreateInputLayout(PosDesc, 1, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayout));
}

JQuadTess::~JQuadTess()
{
	ReleaseCOM(Tech)
	ReleaseCOM(WorldViewProj)
	ReleaseCOM(World)
	ReleaseCOM(EyePosW)
}