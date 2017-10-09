#include <sstream>
#include <fstream>
#include <vector>

#include "Shader.h"
#include "JRenderer.h"

Shader::Shader(ID3D11Device* device, const std::string filename)
{
	std::ifstream fin(L"./FX/Generic.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, device, &m_FX));
}

Shader::~Shader()
{

}
