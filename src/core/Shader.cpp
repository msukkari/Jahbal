#include <sstream>
#include <fstream>
#include <vector>

#include "core/Shader.h"
#include "core/JRenderer.h"

Shader::Shader(ID3D11Device* device)
{
	m_deviceReference = device;
}

Shader::~Shader()
{
	ReleaseCOM(m_FX);
	ReleaseCOM(m_InputLayout);
}

void Shader::loadFX(std::string path)
{
	std::ifstream fin(path, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, m_deviceReference, &m_FX));
}

