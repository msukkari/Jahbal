#include "shaders/JBillboard.h"
#include "core/JRenderer.h"

JBillboard::JBillboard(ID3D11Device* device)
	: Shader(device)
{
	loadFX("./FX/Billboard.fxo");
	Tech = m_FX->GetTechniqueByName("Tech");

	// cbPerObject
	ViewProj = m_FX->GetVariableByName("gViewProj")->AsMatrix();
	Mat = m_FX->GetVariableByName("gMaterial");

	// cbPerFrame
	DirectionalLight = m_FX->GetVariableByName("gDLight");
	PointLight = m_FX->GetVariableByName("gPLight");
	EyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();

	// textures
	DiffuseMap = m_FX->GetVariableByName("gDiffuseMap")->AsShaderResource();

	D3D11_INPUT_ELEMENT_DESC PosSizeDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_deviceReference->CreateInputLayout(PosSizeDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayout));
}

JBillboard::~JBillboard()
{
	ReleaseCOM(Tech)
	ReleaseCOM(ViewProj)
	ReleaseCOM(EyePosW)
	ReleaseCOM(DirectionalLight)
	ReleaseCOM(PointLight)
	ReleaseCOM(Mat)
	ReleaseCOM(DiffuseMap)
}