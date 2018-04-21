#include "ShaderManager.h"
#include "Shader.h"
#include "JGeneric.h"
#include "JBillboard.h"
#include "JQuadTess.h"
#include "JTerrain.h"

ShaderManager* ShaderManager::m_pInstance = nullptr;

void ShaderManager::Init(ID3D11Device* device)
{
	m_JGeneric = new JGeneric(device);
	m_JBillboard = new JBillboard(device);
	m_JQuadTess = new  JQuadTess(device);
	m_JTerrain = new JTerrain(device);
}

void ShaderManager::ShutDown()
{
	delete m_JGeneric;
	delete m_JBillboard;
	delete m_JQuadTess;
	delete m_JTerrain;
}