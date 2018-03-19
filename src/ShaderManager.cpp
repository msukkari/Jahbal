#include "ShaderManager.h"
#include "Shader.h"
#include "JGeneric.h"
#include "JBillboard.h"
#include "JQuadTess.h"

ShaderManager* ShaderManager::m_pInstance = nullptr;

void ShaderManager::Init(ID3D11Device* device)
{
	m_JGeneric = new JGeneric(device);
	m_JBillboard = new JBillboard(device);
	m_JQuadTess = new  JQuadTess(device);
}

void ShaderManager::ShutDown()
{
	delete m_JGeneric;
	delete m_JBillboard;
}