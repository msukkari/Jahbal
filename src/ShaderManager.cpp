#include "ShaderManager.h"
#include "Shader.h"
#include "JGeneric.h"
#include "JBillboard.h"

ShaderManager* ShaderManager::m_pInstance = nullptr;

void ShaderManager::Init(ID3D11Device* device)
{
	m_JGeneric = new JGeneric(device);
	m_JBillboard = new JBillboard(device);
}

void ShaderManager::ShutDown()
{
	delete m_JGeneric;
	delete m_JBillboard;
}