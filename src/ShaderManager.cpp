#include "ShaderManager.h"
#include "Shader.h"
#include "JGeneric.h"

ShaderManager* ShaderManager::m_pInstance = nullptr;

void ShaderManager::Init(ID3D11Device* device)
{
	m_JGeneric = new JGeneric(device, "./FX/Generic.fxo");
}

void ShaderManager::ShutDown()
{

}