#include "JGeneric.h"
#include "JRenderer.h"

JGeneric::JGeneric(ID3D11Device* device, std::string filename)
	: Shader(device, filename)
{
	Tech = m_FX->GetTechniqueByName("Tech");


	// cbPerObject
	WorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World = m_FX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = m_FX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	EyePosW = m_FX->GetVariableByName("gEyePosW")->AsVector();
	Mat = m_FX->GetVariableByName("gMaterial");

	// cbPerFrame
	DirectionalLight = m_FX->GetVariableByName("gDLight");

	D3D11_INPUT_ELEMENT_DESC PosNorUVDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24,
		D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(device->CreateInputLayout(PosNorUVDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayout));
}