#include <sstream>
#include <fstream>
#include <vector>

#include "Shader.h"
#include "VisualComponent.h"

Shader::Shader(VisualComponent* owner)
{
	m_ComponentOwner = owner;

	InitFX();
	InitInputLayout();
}

void Shader::InitFX()
{
	std::ifstream fin(L"./FX/Generic.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, m_ComponentOwner->GetGFXDevice(), &m_FX));

	m_Tech = m_FX->GetTechniqueByName("Tech");
	m_MVP = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void Shader::InitInputLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_ComponentOwner->GetGFXDevice()->CreateInputLayout(vertexDesc, 1, passDesc.pIAInputSignature, 
		passDesc.IAInputSignatureSize, &m_InputLayout));


}
Shader::~Shader()
{
}
